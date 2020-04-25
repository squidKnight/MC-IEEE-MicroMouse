#include <stdio.h>

int getID(int position[2])
{
	int nodeID = position[0] + (position[1] * 16) + 1;
	fprintf(stderr, "\t\tNode ID: %d\n", nodeID);
	fflush(stderr);
	return nodeID;
}