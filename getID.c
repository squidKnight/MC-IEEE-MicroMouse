/*
Written by squidKnight
Last modified: 06/05/20
Purpose: generate unique ID for a node based on it's x-y coords.
Status: FINISHED, TESTED
*/

#include <stdio.h>

short int getID(short int position[2])
{
	short int nodeID = position[0] + (position[1] * 16) + 1;
	fprintf(stderr, "\t\tNode ID: %d\n", nodeID);
	fflush(stderr);
	return nodeID;
}