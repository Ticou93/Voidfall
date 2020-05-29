#pragma once
#include <stdint.h>

/*
TO DO Suggestion: Make it so that struct uses the MAX_ENTITIES variable for array sizes
*/
struct Transform
{
	// X, Y for position of cube.
	Vec3 position;
	Vec3 rotation;
};