/*
Written by Mathazzar
Last modified: 06/05/20
Purpose: return the micromouse from an arbitrary point in the maze to start.
*/

#include <stdbool.h>
#include <stdio.h>
#include "API.h" //only needed for simulator use
#include "mouseDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)
short int nodeCheck();
short int updateDir(short int direction, short int relativeChange);
short int getID(short int position[2]);
short int stackCheck(short int nodeList[NODES][DATA], short int nodeCurrent); //adds new node short into correct rank in stack based on distance

/*short int rtb(short int nodeList[NODES][DATA], short int pathList[NODES / 8], short int position[2], short int direction, short int nodeNext)
INPUTS: short int nodeList[NODES][DATA], short int pathList[NODES / 8], short int position[2], short int direction, short int nodeNext
	nodeList: the nodeList array.
	pathList: the pathList array.
	position: the position array.
	direction: the current orientation of the micromouse.
	nodeNext: the desired endpoint.
RETURNS: short int nodeList[NODES][DATA], short int pathList[NODES / 8], short int position[2], short int dire
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
	Manipulates the position array directly.
*/
short int rtb(short int nodeList[NODES][DATA], short int pathList[NODES / 8], short int position[2], short int direction, short int nodeNext)
{
	
	simLog("Returning to base...");
	short int dire = direction;
	short int nodeCurrent = stackCheck(nodeList, pathList[0]);
	short int nodeID = getID(position);

	if (nodeID != nodeList[nodeCurrent][NODEID])
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

		dire = changeDir(dire, directionNext(nodeList[nodeCurrent], pathList[++x]));
		if (dire == -1)
		{
			simLog("FATAL ERROR: backpath not among available routes.");
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
			simLog("FATAL ERROR: didn't expect a wall.");
			break;
		}
	}

	return dire;
}