/*
Written by Mathazzar
Last modified: 10/15/20
Purpose: choose next not fully explored node and travel to it.
Status: UNFINISHED, NOT TESTED
*/

#include "API.h"
#include <stdio.h>
#include "mouseDefs.h"
#include "nodeDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)
short int updateDir(short int direction, short int relativeChange); //updates the direction the micromouse is facing
short int getID(short int position[2]); //generates unique ID for a node based on it's x-y coords
short int changeDir(short int direction, short int newDirection); //changes the dirction the micromouse to a specific desired direction
void stackPath(bool nodeList[NODES][DATA], short int pathList[NODES / 2], short int holdList[NODES], short int nodeCurrent, short int nodeNext); //finds shortest path to a given node from a given node using a minimum spanning tree
void pathTree(bool nodeList[NODES][DATA], short int holdList[NODES], short int nodeCurrent);  //generates minimum spanning tree from nodeCurrent
short int rtb(bool nodeList[NODES][DATA], short int pathList[NODES / 2], short int position[2], short int direction, short int nodeNext);

/*short int pathChooseAlt(bool nodeList[NODES][DATA], short int nodeCurrent,short int direction, short int position[2])
INPUTS: bool nodeList[NODES][DATA], short int nodeCurrent, short int direction, short int position[2]
	nodeList: the nodeList array.
	nodeCurrent: nodeID of the current node on the stack that the micromouse is at and must choose the next path for.
	direction: the current direction of the micromouse in relation to its original orientation.
	position: the current position of the micromouse. Implemented for use with the position array in scan.c
RETURNS: short int direction, short int position[2], bool nodeList[NODES][DATA]
	direction: the new direction as decided by rtb(), which is returned.
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
	Manipulates the position array passed to it directly via rtb().
*/
short int pathChooseAlt(bool nodeList[NODES][DATA], short int nodeCurrent, short int direction, short int position[2])
{
	simLog("Searching for unexplored path along backpath...");

	short int nodeID = getID(position);
	if (nodeCurrent != nodeID)
	{
		simLog("FATAL ERROR: nodeID missmatch.");
		return direction;
	}
	//find next node not fully explored
	short int nodeNextID = nodeID;
	short int holdList[NODES];
	pathTree(nodeList, holdList, nodeID);
	short int minDist = INFINITY;
	for (int i = 0; i < NODES; i++)
	{
		if (nodeList[i][EXP_T] || nodeList[i][EXP_R] || nodeList[i][EXP_B] || nodeList[i][EXP_L])
		{
			if (holdList[i] < minDist)
			{
				nodeNextID = node(i) + 1;
				minDist = holdList[i];
			}
		}
	}
	if (minDist == INFINITY)
	{
		simLog("CRITICAL ERROR: no nodes found with an unexplored route shorter than INFINITY.");
		return direction;
	}
	else
	{
		fprintf(stderr, "nodeID %d selected as nodeNext\n", nodeNextID);
		fflush(stderr);
	}

	//calculate route from nodeNext to start
	simLog("calculating route from nodeNext to current node...");
	short int pathList[NODES / 2];
	stackPath(nodeList, pathList, holdList, nodeCurrent, nodeNextID);

	//traverse maze along chosen path to nodeNext
	return rtb(nodeList, pathList, position, direction, nodeNextID);
}
