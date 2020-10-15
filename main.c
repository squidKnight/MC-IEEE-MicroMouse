/*
Written by squidKnight, Mathazzar
Last modified: 10/15/20
Purpose: run the program on the simulator, which requires a main.c

NOTE: ONLY USED FOR SIMULATOR, NOT FOR USE ON ARDUINO!!
*/

#include <stdio.h>
#include "mouseDefs.h"
#include "API.h" //only needed for simulator use

bool nodeList[NODES][DATA];

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)
void nodeInit(bool nodeList[NODES][DATA]); //initialize nodeList
short int scan(bool nodeList[NODES][DATA], short int position[2], short int direction); //scans the maze
void pathTree(bool nodeList[NODES][DATA], short int holdList[NODES], short int nodeCurrent); //generates minimum spanning tree from nodeCurrent
void stackPath(bool nodeList[NODES][DATA], short int pathList[NODES / 4], short int holdList[NODES], short int nodeCurrent, short int nodeNext); //finds shortest path to a given node from a given node using a minimum spanning tree
short int getID(short int position[2]); //generates unique ID for a node based on it's x-y coords
short int rtb(bool nodeList[NODES][DATA], short int pathList[NODES / 4], short int position[2], short int direction, short int nodeNext); //moves the micromouse to nodeNext from anywhere in the maze
short int updateDir(short int direction, short int relativeChange); //updates the direction the micromouse is facing

int main()
{
	short int position[2] = { 0, 0 };
	short int direction = 0;

	//explore maze exaustively
	nodeInit(nodeList);
	direction = scan(nodeList, position, direction);
	for (int i = 0; i < NODES; i++)
	{
		fprintf(stderr, "%d:\t%d, %d, %d, %d;\t%d, %d, %d, %d\n", i+1, nodeList[i][WAL_T], nodeList[i][WAL_R], nodeList[i][WAL_B], nodeList[i][WAL_L], nodeList[i][EXP_T], nodeList[i][EXP_R], nodeList[i][EXP_B], nodeList[i][EXP_L]);
		fflush(stderr);
	}

	//return to start
	simLog("Going back to start...");
	short int holdList[NODES];
	short int nodeCurrent = getID(position);
	pathTree(nodeList, holdList, nodeCurrent);
	short int pathList[NODES / 4];
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
	if (holdList[119] < minDist) //check distance to nodeID 120 (bottom left center)
	{
		minDist = holdList[119];
		nodeCurrent = 119; //reuse nodeCurrent as nodeNext
	}
	if (holdList[120] < minDist) //check distance to nodeID 121 (bottom right center)
	{
		minDist = holdList[120];
		nodeCurrent = 120; //reuse nodeCurrent as nodeNext
	}
	if (holdList[135] < minDist) //check distance to nodeID 136 (top left center)
	{
		minDist = holdList[135];
		nodeCurrent = 135; //reuse nodeCurrent as nodeNext
	}
	if (holdList[136] < minDist) //check distance to nodeID 137 (top right center)
	{
		minDist = holdList[136];
		nodeCurrent = 136; //reuse nodeCurrent as nodeNext
	}
	if (minDist == INFINITY)
		simLog("FATAL ERROR: center was not found.");
	else
	{
		stackPath(nodeList, pathList, holdList, getID(position), nodeCurrent);
		//^^^^final step to define points on 16x16 grid for smoothing, further code here can only be done for the simulator's sake.
		//Final product code can't be tested in the simulator beyond this point, and requires either a physical micromouse and maze, or other simulation software to test with.
		direction = rtb(nodeList, pathList, position, direction, nodeCurrent);
	}

	return 0;
}
