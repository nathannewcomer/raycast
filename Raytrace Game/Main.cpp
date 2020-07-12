#include "SDL.h"
#include <stdio.h>
#include <math.h>
#include "Maps.h"
#include "Player.h"
#include "Drawing.h"
#include "Helpers.h"
#include "Definitions.h"

/* draw line coming out of player
void drawLine(SDL_Renderer* r) {
    int xcenter = px + player.w / 2, ycenter = py + player.h / 2;
    SDL_SetRenderDrawColor(r, 255, 255, 0, 255);
    SDL_RenderDrawLine(r, xcenter, ycenter, xcenter + pdx*5, ycenter + pdy * 5);

}*/

int main(int argc, char* argv[])
{
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Surface* windowScreen = NULL;
    const Uint8* keystate;

    px = 300;
    py = 300;
    pdx = cos(pa);
    pdy = sin(pa);

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Raytrace", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sWidth, sHeight, 0);
    windowScreen = SDL_GetWindowSurface(window);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    // game loop
    while (1) {

        /* event handling */
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    break;
                }
            }
        }

        // --- handle keyboard presses ---

        int prevx = px;     // the x position before the player moves
        int prevy = py;     // the y position before the player moves

        keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_W]) {             // forward
            px += pdx * speed;
            py += pdy * speed;
        }
        if (keystate[SDL_SCANCODE_S]) {             // backward
            px -= pdx * speed;
            py -= pdy * speed;
        }
        if (keystate[SDL_SCANCODE_A]) {
            if (keystate[SDL_SCANCODE_LSHIFT]) {    // strafe left
                float theta = pa - PI2;     // add 90 degrees to player angle
                px += cos(theta) * speed;   // break vector into x & y
                py += sin(theta) * speed;
            }
            else {
                pa -= rotationSpeed;
                if (pa < 0) {
                    pa += 2 * PI;
                }
                pdx = cos(pa);
                pdy = sin(pa);
            }
        }
        if (keystate[SDL_SCANCODE_D]) {             // strafe right
            if (keystate[SDL_SCANCODE_LSHIFT]) {
                float theta = pa + PI2;     // add 90 degrees to player angle
                px += cos(theta) * speed; // see above
                py += sin(theta) * speed;
            }
            else {
                pa += rotationSpeed;
                if (pa > 2 * PI) {
                    pa -= 2 * PI;
                }
                pdx = cos(pa);
                pdy = sin(pa);
            }
        }

        printf("x: %f\ny: %f\n\n", px, py);

        if (map[(int)(py / mapS) * 8 + (int)(px / mapS)]) {   // move player to previous position if in a tile
            px = prevx;
            py = prevy;
        }
      
        /* clear the screen */
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);

        /* drawing */
        //drawMap2D(renderer);
        //drawLine(renderer);
        //drawPlayer(renderer);
        drawRays2D(renderer);

        /* show the contents of the screen */
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
