/*
Written by Mathazzar
Last modified: 05/29/20
Purpose: identify the direction of the current node's backpath to prevent longer path to same node being taken.
Status: NOT FINISHED, NOT TESTED
*/

#include <stdio.h>
#include "mouseDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)

/*short int directionBack(int nodeCurrent[DATA])
INPUTS: int nodeCurrent[DATA]
	nodeCurrent: the node whose backpath is to be determined.
RETURNS: short int dire
	dire: new variable that gives the direction of nodeCurrent's backpath.
NOTES:
	Designed to prevent nodes with multiple direct connections to eachother from resulting in a longer distance than optimal being picked.
	Implemented to be called as:
		direction = changeDir(direction, directionBack(nodeList[nodeCurrent]));
		to update direction to new direction.
	0=up/straight, 1=right, 2=down/about face, 3=left
CAUTION:
	dire is initialized to -1, which is an invalid direction. If NODEID_P isn't equal to any of the directional NODEIDs or the correct path is impossibly longer than INFINITY, it will return the -1 and break later functions.
*/
short int directionBack(int nodeCurrent[DATA])
{
	short int dire = -1;
	int shortestPath = INFINITY;

	if (nodeCurrent[NODEID_P] == nodeCurrent[NODEID_T])
		if (nodeCurrent[DIST_T] < shortestPath)
		{
			simLog("Top appears to be backpath.");
			dire = 0;
			shortestPath = nodeCurrent[DIST_T];
		}
	if (nodeCurrent[NODEID_P] == nodeCurrent[NODEID_R])
		if (nodeCurrent[DIST_R] < shortestPath)
		{
			simLog("Right appears to be backpath.");
			dire = 1;
			shortestPath = nodeCurrent[DIST_R];
		}
	if (nodeCurrent[NODEID_P] == nodeCurrent[NODEID_B])
		if (nodeCurrent[DIST_B] < shortestPath)
		{
			simLog("Botttom appears to be backpath.");
			dire = 2;
			shortestPath = nodeCurrent[DIST_B];
		}
	if (nodeCurrent[NODEID_P] == nodeCurrent[NODEID_L])
		if (nodeCurrent[DIST_L] < shortestPath)
		{
			simLog("Left appears to be backpath.");
			dire = 3;
			shortestPath = nodeCurrent[DIST_L];
		}

	if (dire == -1)
	{
		fprintf(stderr, "CRITICAL ERROR: backpath not among available routes:\nNODEID: %d, NODEID_P: %d, NODEID_T: %d, NODEID_R: %d, NODEID_B:%d, NODEID_L:%d.\n", nodeCurrent[NODEID], nodeCurrent[NODEID_P], nodeCurrent[NODEID_T], nodeCurrent[NODEID_R], nodeCurrent[NODEID_B], nodeCurrent[NODEID_L]);
		fflush(stderr);
	}
	return dire;
}