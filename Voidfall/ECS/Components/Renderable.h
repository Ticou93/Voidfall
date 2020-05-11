#pragma once
#include <stdint.h>

/*
TO DO Suggestion: Make it so that struct uses the MAX_ENTITIES variable for array sizes
*/

// Set amount of triangles to be drawn
const int triangles = 2;
const int lines = triangles * 4;
const int rows = lines * 3;
const int sides = rows * 6;

struct Renderable
{
	// X, Y, Z axis and colour in 0x0000 format
	float xyzc[sides];
};