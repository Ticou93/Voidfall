#pragma once
#include <stdint.h>

/*
TO DO Suggestion: Make it so that struct uses the MAX_ENTITIES variable for array sizes
*/
struct Faction
{
	// Flag that decides if the entity is allied or enemy (or in this case the player or not)
	bool isEnemy;
};