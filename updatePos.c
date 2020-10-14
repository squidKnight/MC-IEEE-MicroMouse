/*
Written by Mathazzar
Last modified: 06/05/20
Purpose: update the position of the micromouse.
Status: FINISHED, TESTED
*/

#include <stdio.h>

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)

/*void updatePos(int position[2], short int direction, short int dist)
	INPUTS: position[2], direction, dist
	position[2]: the current position of the micromouse. Implemented for use with the position array in scan.c
	direction: the current direction of the micromouse in relation to its original orientation.
	dist: the distance the micromouse has traveled since its last change in direction
RETURNS:
NOTES:
	implemented to be called directly.
	primary function to intract with the position array.
CAUTION:
	Manipulates the position array passed to it directly.
*/
void updatePos(short int position[2], short int direction, short int dist)
{
	//fprintf(stderr, "moving from (%d, %d, %d) %d units to ", position[0], position[1], direction, dist);
	//fflush(stderr);
	switch (direction)
	{
	case 0: //if facing up
		position[1] += dist;
		break;
	case 1: //if facing right
		position[0] += dist;
		break;
	case 2: //if facing down
		position[1] -= dist;
		break;
	case 3: //if facing left
		position[0] -= dist;
		break;
	default:
		simLog("DIRECTION ERROR");
		break;
	}
	//fprintf(stderr, "(%d, %d, %d) \n", position[0], position[1], direction);
	//fflush(stderr);
	if (position[0] < 0 || position[0] >= 16)
	{
		simLog("X position ERROR");
	}
	if (position[1] < 0 || position[1] >= 16)
	{
		simLog("Y position ERROR");
	}
}