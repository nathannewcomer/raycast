#pragma once
#include "SDL.h"
#include "Maps.h"
#include "Player.h"

// --- Variables ---
extern SDL_Rect wall;
extern SDL_Rect wall3d;
extern SDL_Rect player;

// --- Functions ---

// draw the player
void drawPlayer(SDL_Renderer* r);

// Draws the 2D map
void drawMap2D(SDL_Renderer* r);

// draw the 3D space
void drawRays2D(SDL_Renderer* renderer);