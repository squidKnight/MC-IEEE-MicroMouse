/*
Written by Mathazzar
Last modified: 05/25/20
Purpose: choose next direction to take at a revisited node.
Status: FINISHED, TESTED
*/

#include "API.h"
#include <stdio.h>
#include <stdbool.h>
#include "mouseDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)
int stackCheck(int nodeList[NODES][DATA], int nodeCurrent); //adds new node into correct rank in stack based on distance
short int updateDir(short int direction, short int relativeChange);

/*short int pathChoose(int nodeList[NODES][DATA], int nodeCurrent,short int direction)
INPUTS: int nodeList[NODES][DATA], int nodeCurrent, short int direction
	nodeList: the nodeList array
	nodeCurrent: rank of the current node on the stack that the micromouse is at and must choose the next path for
	direction: the current direction of the micromouse in relation to its original orientation.
RETURNS: short int dire, int nodeList[NODES][DATA]
	dire: a new variable for the new direction, which is returned.
	nodeList: updates the explored directions directly.
NOTES:
	implemented to be called as:
		direction = pathChoose(nodeList, nodeCurrent, direction, position);
		to set direction to new orientation once it has returned.
	Doesn't check for direct loopbacks, they need to be checked externally before this function executes.
CAUTION:
	Manipulates the nodeList array passed to it directly.
*/
short int pathChoose(int nodeList[NODES][DATA], int nodeCurrent, short int direction)
{
	short int dire = direction;
	short int front, right, back, left;
	switch (dire) //set directions for current orientation
	{
	case 0:
		front = NODEID_T;
		right = NODEID_R;
		back = NODEID_B;
		left = NODEID_L;
		break;
	case 1:
		front = NODEID_R;
		right = NODEID_B;
		back = NODEID_L;
		left = NODEID_T;
		break;
	case 2:
		front = NODEID_B;
		right = NODEID_L;
		back = NODEID_T;
		left = NODEID_R;
		break;
	case 3:
		front = NODEID_L;
		right = NODEID_T;
		back = NODEID_R;
		left = NODEID_B;
		break;
	}
	fprintf(stderr, "nodeCurrent: %d, nodeID: %d,\t\tfront: %d, right: %d, back: %d, left: %d\n", nodeCurrent, nodeList[nodeCurrent][NODEID], nodeList[nodeCurrent][front], nodeList[nodeCurrent][right], nodeList[nodeCurrent][back], nodeList[nodeCurrent][left]);
	fflush(stderr);

	//Choose next available route not previously traveled
	if (nodeList[nodeCurrent][front] == 0) //if front unexplored
	{
		simLog("Picking unexplored straight.");
	}
	else if (nodeList[nodeCurrent][left] == 0) //if left unexplored
	{
		simLog("Picking unexplored left.");
		API_turnLeft();
		dire = updateDir(dire, 3);
	}
	else if (nodeList[nodeCurrent][right] == 0) //if right unexplored
	{
		simLog("Picking unexplored right.");
		API_turnRight();
		dire = updateDir(dire, 1);
	}
	
	return dire;
}
