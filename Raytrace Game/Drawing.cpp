#include "SDL.h"
#include "Maps.h"
#include "Drawing.h"
#include "Helpers.h"
#include "Player.h"
#include "Definitions.h"

SDL_Rect wall = { 0, 0, mapS - 1, mapS - 1 };
SDL_Rect wall3d = { 0, 0, 6, 8 };
SDL_Rect player = { px, py, 10, 10 };

// draw the player to the screen
void drawPlayer(SDL_Renderer* r) {
    player.x = (int)px;
    player.y = (int)py;

    SDL_SetRenderDrawColor(r, 255, 255, 0, 255);
    SDL_RenderFillRect(r, &player);

}

/* draws the map */
void drawMap2D(SDL_Renderer* r) {
    int x, y, xo = 100, yo = 100;
    for (y = 0; y < mapY; y++) {
        for (x = 0; x < mapX; x++) {

            // position the wall
            xo = x * mapS;
            yo = y * mapS;
            wall.x = xo + 1;
            wall.y = yo + 1;

            if (map[y * mapX + x] == 1) {   // wall
                SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
            }
            if (map[y * mapX + x] == 0) {   // empty space
                SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
            }

            SDL_RenderFillRect(r, &wall);
        }
    }
}

// draws the 3D world
void drawRays2D(SDL_Renderer* renderer) {
    int xcenter = px + player.w / 2, ycenter = py + player.h / 2;

    int r = 0;      // number of rays (used in for loop)
    int mx = 0;
    int my = 0;
    int mp = 0;
    int dof;        // depth of field
    float rx = 0;
    float ry = 0;
    float ra;       // ray angle
    float xo = 0;
    float yo = 0;
    float disT;

    // initialize first ray angle
    ra = pa - DEGREE * 30;

    if (ra < 0) {
        ra += 2 * PI;
    }
    else if (ra > 2 * PI) {
        ra -= 2 * PI;
    }

    for (r = 0; r < 120; r++) {
        // --- Check horizontal lines ---

        dof = 0;
        float disH = 1000000;
        float hx = px;
        float hy = py;
        float aTan = -1 / tan(ra);

        if (ra > PI) {  // looking up
            ry = (((int)py >> 6) << 6) - 0.0001;
            rx = (py - ry) * aTan + px;
            yo = -64;
            xo = -yo * aTan;
        }
        else if (ra < PI) {  // looking down
            ry = (((int)py >> 6) << 6) + 64;
            rx = (py - ry) * aTan + px;
            yo = 64;
            xo = -yo * aTan;
        }
        if (ra == 0 || ra == PI) { // looking straight left or right
            rx = px;
            ry = py;
            dof = 8;
        }
        while (dof < 8) {
            mx = (int)(rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * mapX + mx;
            if (mp < mapX * mapY && mp > 0 && map[mp] == 1) { // hit wall
                hx = rx;
                hy = ry;
                disH = distance(px, py, hx, hy);
                dof = 8;
            }
            else {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }


        // --- Check vertical lines ---

        dof = 0;
        float disV = 1000000, vx = px, vy = py;
        float nTan = -tan(ra);

        if (ra > PI2 && ra < PI3) {  // looking left
            rx = (((int)px >> 6) << 6) - 0.0001;
            ry = (px - rx) * nTan + py;
            xo = -64;
            yo = -xo * nTan;
        }
        else if (ra < PI2 || ra > PI3) {  // looking right
            rx = (((int)px >> 6) << 6) + 64;
            ry = (px - rx) * nTan + py;
            xo = 64;
            yo = -xo * nTan;
        }
        if (ra == 0 || ra == PI) { // looking straight up or down
            rx = px;
            ry = py;
            dof = 8;
        }

        while (dof < 8) {
            mx = (int)(rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * mapX + mx;
            if (mp < mapX * mapY && mp > 0 && map[mp] == 1) { // hit wall
                dof = 8;
                vx = rx;
                vy = ry;
                disV = distance(px, py, vx, vy);
            }
            else {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }

        // pick the shortest line (horizontal or vertical)
        if (disV < disH) {          // vertical wall hit
            rx = vx;
            ry = vy;
            disT = disV;
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        else if (disH <= disV) {    // horizontal wall hit
            rx = hx;
            ry = hy;
            disT = disH;
            SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
        }

        // --- Draw 3D walls ---
        float ca = pa - ra;

        if (ca < 0) {
            ca += 2 * PI;
        }
        else if (ca > 2 * PI) {
            ca -= 2 * PI;
        }
        disT = disT * cos(ca);  // fix fisheye
                
        float lineH = (mapS * 320) / disT;  // line height
        float lineO = 160 - lineH / 2;      // line offset

        // actually draw the 3d walls
        int rayWidth = sWidth / 120;  // ray width is screen width in pixels / number of rays
        wall3d.x = r * rayWidth;// +530 - wall3d.w;
        wall3d.y = lineO;
        wall3d.h = lineH;
        SDL_RenderFillRect(renderer, &wall3d);

        // draw angle line
        //SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        //SDL_RenderDrawLine(renderer, xcenter, ycenter, rx, ry);

        // increase ra by 1/2 degree
        ra += DEGREE / 2;

        // make sure ra is a correct unit in the unit circle (wrap value around)
        if (ra < 0) {
            ra += 2 * PI;
        }
        else if (ra > 2 * PI) {
            ra -= 2 * PI;
        }

    }
}