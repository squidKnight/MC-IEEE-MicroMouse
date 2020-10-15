/*
Written by Mathazzar
Last modified: 10/15/20
Purpose: return the micromouse from an arbitrary point in the maze to nodeNext.
*/

#include <stdio.h>
#include "API.h" //only needed for simulator use
#include "mouseDefs.h"
#include "nodeDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)
short int changeDir(short int direction, short int newDirection); //updates the direction the micromouse is facing
short int getID(short int position[2]); //generates unique ID for a node based on it's x-y coords
void updatePos(short int position[2], short int direction, short int dist); //updates the position of the micromouse

static short int directionNext(bool nodeCurrent[DATA], short int rank, short int pathNext);

/*short int rtb(bool nodeList[NODES][DATA], short int pathList[NODES / 8], short int position[2], short int direction, short int nodeNext)
INPUTS: bool nodeList[NODES][DATA], short int pathList[NODES / 8], short int position[2], short int direction, short int nodeNext
	nodeList: the nodeList array.
	pathList: the pathList array.
	position: the position array.
	direction: the current orientation of the micromouse.
	nodeNext: nodeID of the desired endpoint.
RETURNS: bool nodeList[NODES][DATA], short int pathList[NODES / 8], short int position[2], short int dire
	nodeList: should return unmodified.
	pathList: should return unmodified.
	position: modifies the position array directly.
	dire: the new orientation of the micrmouse that it must be set to.
NOTES:
	Implemented to have nodeList and pathList arrays already generated and filled out.
	Implemented to be called as:
		direction = short int rtb(nodeList, pathList, position, direction, nodeNext);
		to update direction to new direction.
CAUTION:
	Interacts with the nodeList array directly.
	Interacts with the pathList array directly.
	Manipulates the position array directly via updatePos().
*/
short int rtb(bool nodeList[NODES][DATA], short int pathList[NODES / 8], short int position[2], short int direction, short int nodeNext)
{

	simLog("Returning to base...");
	short int dire = direction;
	short int nodeCurrent = node(pathList[0] - 1);  //find position in nodeList array of the start of the pathList
	short int nodeID = getID(position);

	if (nodeID != (node(nodeCurrent) + 1))
	{
		simLog("FATAL ERROR: NodeID missmatch.");
		return dire;
	}

	//traverse maze along chosen path to nodeNext
	short int x = 0;
	while (nodeID != nodeNext)
	{
		if (pathList[x] == INFINITY)
		{
			fprintf(stderr, "FATAL ERROR: nodes in stack exceeded: %d.", x);
			fflush(stderr);
			break;
		}
		else if (nodeID != pathList[x])
		{
			simLog("FATAL ERROR: expected route not taken.");
			break;
		}

		dire = changeDir(dire, directionNext(nodeList[nodeCurrent], nodeCurrent, pathList[++x]));
		if (dire == -1)
		{
			simLog("FATAL ERROR: backpath not among available routes.");
			return dire;
		}

		if (!API_wallFront())
		{
			API_moveForward();
			updatePos(position, dire, 1);

			nodeID = getID(position);
			nodeCurrent = stackCheck(nodeList, nodeID);
		}
		else
		{
			simLog("FATAL ERROR: didn't expect a wall.");
			break;
		}
	}

	return dire;
}

static short int directionNext(bool nodeCurrent[DATA], short int rank, short int pathNext)
{
	short int dire = -1;
	short int next = node(pathNext - 1);
	if ((node_T(rank) == next) && nodeCurrent[WAL_T])
		dire = 0;
	else if ((node_R(rank) == next) && nodeCurrent[WAL_R])
		dire = 1;
	else if ((node_B(rank) == next) && nodeCurrent[WAL_B])
		dire = 2;
	else if ((node_L(rank) == next) && nodeCurrent[WAL_L])
		dire = 3;
	else
		simLog("CRITICAL ERROR: nodeNext is not adjacent to nodeCurrent");
	return dire;
}
