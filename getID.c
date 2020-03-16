/*
Written by squidKnight
Last modified: 03/16/20
Purpose: gets the ID of the current node
Status: finished, tested

NOTE: ONLY COMPATABLE IN SIMULATOR!! Need to translate and incorperate finished movement/sensor functions once algorithm in stable
*/

#include "API.h" //only needed for simulator use	
#include <stdio.h>

int translate(int x, int y) //translates values from cartesian system used in simulator to 16x16 matrix node IDs
{
	int nodeID = (x + (y * 16) + 1); //conversion from cartesian to matrix
	fprintf(stderr, "\t\tNode ID: %d\n", nodeID);
	fflush(stderr);
	return nodeID;
}

int getID(int direction, int dist, int position[2])
{
	//determine change based on direction
	if(direction == 0) //if facing up
		position[1] += dist; //y position increased (normally, this would be incremented in accordance to the matrix by recorded traveled distance)
	if(direction == 1) //if facing down
		position[1] -= dist;
	if(direction == 2) //if facing right
		position[0] += dist; //x position increased
	if(direction == 3) //if facing left
		position[0] -= dist;

	//translate to matrix format, this can be removed once outside of simulator
	return translate(position[0], position[1]);
}
