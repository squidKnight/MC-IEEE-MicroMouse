/*
Written by squidKnight, Mathazzar
Last modified: 06/05/20
Purpose: run the program on the simulator, which requires a main.c

NOTE: ONLY USED FOR SIMULATOR, NOT FOR USE ON ARDUINO!!
*/

#include "mouseDefs.h"
#include <stdio.h>

short int nodeList[NODES][DATA];

void nodeInit(short int nodeList[NODES][DATA]); //initialize nodeList
short int scan(short int nodeList[NODES][DATA], short int position[2], short int direction); //scans the maze
void pathTree(short int nodeList[NODES][DATA], short int holdList[NODES], short int nodeCurrent); //generates minimum spanning tree from current node
void stackPath(short int nodeList[NODES][DATA], short int pathList[NODES / 8], short int holdList[NODES], short int nodeCurrent, short int nodeNext);
short int getID(short int position[2]);
short int rtb(short int nodeList[NODES][DATA], short int pathList[NODES / 8], short int position[2], short int direction, short int nodeNext);
short int updateDir(short int direction, short int relativeChange);

int main()
{
	short int position[2] = { 0, 0 };
	short int direction = 0;

	//explore maze exaustively
	nodeInit(nodeList);
	direction = scan(nodeList, position, direction);
	for (int i = 0; i < NODES; i++)
	{
		fprintf(stderr, "%d;\t%d, %d, %d, %d;\t%d, %d, %d, %d\n", i, nodeList[i][NODEID], nodeList[i][NODEID_T], nodeList[i][NODEID_R], nodeList[i][NODEID_B], nodeList[i][NODEID_L], nodeList[i][DIST_T], nodeList[i][DIST_R], nodeList[i][DIST_B], nodeList[i][DIST_L]);
		fflush(stderr);
	}

	//return to start
	short int holdList[NODES];
	short int nodeCurrent = getID(position);
	pathTree(nodeList, holdList, nodeCurrent);
	short int pathList[NODES / 8];
	stackPath(nodeList, pathList, holdList, nodeCurrent, 1);
	direction = rtb(nodeList, pathList, position, direction, 1);
	while (API_wallFront())
	{
		API_turnRight();
		direction = updateDir(direction, 1);
	}

	//go to goal
	nodeCurrent = getID(position);
	pathTree(nodeList, holdList, nodeCurrent);
	short int minDist = INFINITY;
	for (int i = 0; i < NODES; i++)
	{
		if ((nodeList[i][NODEID] == 120) || (nodeList[i][NODEID] == 121) || (nodeList[i][NODEID] == 136) || (nodeList[i][NODEID] == 137))
			if (holdList[i] < minDist)
			{
				minDist = holdList[i];
				nodeCurrent = nodeList[i][NODEID]; //reuse nodeCurrent as nodeNext
			}
	}
	if (minDist == INFINITY)
		simLog("FATAL ERROR: center was not found.");
	else
	{
		stackPath(nodeList, pathList, holdList, getID(position), nodeCurrent);
		direction = rtb(nodeList, pathList, position, direction, nodeCurrent);
	}

	return 0;
}
