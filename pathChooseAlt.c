/*
Written by Mathazzar
Last modified: 06/05/20
Purpose: choose next not fully explored node and travel to it.
Status: FINISHED, TESTED
*/

#include "API.h"
#include <stdio.h>
#include <stdbool.h>
#include "mouseDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)
short int stackCheck(short int nodeList[NODES][DATA], short int nodeCurrent); //adds new node into correct rank in stack based on distance
short int updateDir(short int direction, short int relativeChange);
short int getID(short int position[2]);
short int pathCheck(short int position[2], short int *dire);
void updatePos(short int position[2], short int direction, short int dist);
short int changeDir(short int direction, short int newDirection);
void stackPath(short int nodeList[NODES][DATA], short int pathList[NODES / 4], short int holdList[NODES], short int nodeCurrent, short int nodeNext);
void pathTree(short int nodeList[NODES][DATA], short int holdList[NODES], short int nodeCurrent);
short int directionNext(short int nodeCurrent[DATA], short int nodeNext);

/*short int pathChooseAlt(short int nodeList[NODES][DATA], short int nodeCurrent,short int direction, short int position[2])
INPUTS: short int nodeList[NODES][DATA], short int nodeCurrent, short int direction, short int position[2]
	nodeList: the nodeList array.
	nodeCurrent: rank of the current node on the stack that the micromouse is at and must choose the next path for.
	direction: the current direction of the micromouse in relation to its original orientation.
	position: the current position of the micromouse. Implemented for use with the position array in scan.c
RETURNS: short int dire, short int nodeList[NODES][DATA]
	dire: a new variable for the new direction, which is returned.
	position: updates the position directly.
	nodeList: updates the explored directions directly.
NOTES:
	implemented to be called as:
		direction = pathChooseAlt(nodeList, nodeCurrent, direction, position);
		to set direction to new orientation once it has returned.
	Doesn't check for direct loopbacks, they need to be checked externally before this function executes.
	Manipulates nodeCurrent directly, though it's immutable and won't be affected when this fuction exits.
	Designed to intentionally crash and return to a higher function when no unexplored paths exist.
	Currently assumes that any backpath possible will always be less than NODES/8(~32) long.
CAUTION:
	Manipulates the nodeList array passed to it directly.
	Manipulates the position array passed to it directly via updatePos().
*/
short int pathChooseAlt(short int nodeList[NODES][DATA], short int nodeCurrent, short int direction, short int position[2])
{
	short int dire = direction;

	simLog("Searching for unexplored path along backpath...");
	short int nodeID = nodeList[nodeCurrent][NODEID];

	nodeID = getID(position);
	//find next node not fully explored
	short int nodeNextID = nodeID;
	short int holdList[NODES];
	pathTree(nodeList, holdList, nodeID);
	short int minDist = INFINITY;
	for (int i = 0; i < NODES; i++)
	{
		if ((nodeList[i][NODEID_T] == 0) || (nodeList[i][NODEID_R] == 0) || (nodeList[i][NODEID_B] == 0) || (nodeList[i][NODEID_L] == 0))
		{
			if (holdList[i] < minDist)
			{
				nodeNextID = nodeList[i][NODEID];
				minDist = holdList[i];
			}
		}
	}
	if (minDist == INFINITY)
	{
		simLog("CRITICAL ERROR: no nodes found with an unexplored route shorter than INFINITY.");
		return dire;
	}

	//calculate route from nodeNext to start
	simLog("calculating route from nodeNext to current node...");
	short int pathList[NODES / 8];
	stackPath(nodeList, pathList, holdList, nodeList[nodeCurrent][NODEID], nodeNextID);

	//traverse maze along chosen path to nodeNext
	short int x = 0;
	while (nodeID != nodeNextID)
	{
		if (pathList[x] == INFINITY)
		{
			fprintf(stderr, "CRITICAL ERROR: nodes in stack exceeded: %d.", x);
			fflush(stderr);
			break;
		}
		else if (nodeID != pathList[x])
		{
			simLog("CRITICAL ERROR: expected route not taken.");
			break;
		}

		dire = changeDir(dire, directionNext(nodeList[nodeCurrent], pathList[++x]));
		if (dire == -1)
		{
			simLog("CRITICAL ERROR: backpath not among available routes.");
			return dire;
		}

		if (!API_wallFront())
		{
			API_moveForward();
			updatePos(position, dire, 1);
			pathCheck(position, &dire);

			nodeID = getID(position);
			nodeCurrent = stackCheck(nodeList, nodeID);
		}
		else
		{
			simLog("CRITICAL ERROR: didn't expect a wall.");
			break;
		}
	}

	return dire;
}
