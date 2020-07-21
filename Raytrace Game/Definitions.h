#pragma once
/*
 * Definitions.h
 * Holds various definitions.
 */

// --- Math ---
const float PI = 3.14159265;                    // pi
const float PI2 = PI / 2;                       // pi / 2
const float PI3 = 3 * PI / 2;                   // 3pi / 2
const float DEGREE = 0.0174533;                 // degree in radians

// --- Drawing ---
const int sWidth = 720;                         // screen width
const int sHeight = 540;                        // screen height
const int numberOfRays = sWidth;                // number of rays to draw
const int fov = 60;                             // field of view of the player

// -- Movement ---
const float speed = 50;                         // how fast the player moves
const float rotationSpeed = 5;                  // how fast the player rotates