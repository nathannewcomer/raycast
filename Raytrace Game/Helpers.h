#pragma once
/*
 * Helpers.h
 * Holds various math constants and functions.
 */

const float PI = 3.14159265;    // pi
const float PI2 = PI / 2;       // pi / 2
const float PI3 = 3 * PI / 2;   // 3pi / 2
const float DEGREE = 0.0174533; // degree in radians

// calculates the distance between 2 points using the pythagorean theorem
float distance(float x1, float y1, float x2, float y2);