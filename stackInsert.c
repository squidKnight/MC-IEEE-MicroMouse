/*
Written by SquidKnight, Mathazzar
Last modified: 05/13/20
Purpose: add the current node to the stack
Status: FINISHED, TESTED
*/

#include <stdbool.h>
#include <stdio.h>
#include "API.h" //only needed for simulator use
#include "mouseDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)

/*int stackInsert(int nodeList[NODES][DATA], int nodeCurrent[DATA])
INPUTS: int nodeList[NODES][DATA], int nodeCurrent[DATA]
	nodeList: the nodeList array
	nodeCurrent: nodeCurrent array
RETURNS: int stack int nodeList[NODES][DATA]
	stack: the position in the nodeList array which the new node was added at.
	nodeList: the nodeList array. Directly maipulated instead of being passed back.
NOTES:
	adds new node into correct rank in stack based on distance.
CAUTION:
	Manipulates the nodeList and nodeCurrent arrays directly.
	The nodeCurrent array will not be useful for other code to manipulate after this function executes;
	the stack variable must be stored externally to directly go to the location in the nodeList array the added node is now stored at.
	Primary function to interact with the nodeList array.
*/
int stackInsert(int nodeList[NODES][DATA], int nodeCurrent[DATA])
{
	simLog("\t\tInserting node to stack...");
	bool rankFound = 0; //becomes 1 when stack rank for new node is found
	int stack;
	int tempArr[DATA]; //temporary storage array to allow for swaping in stack
	for (int i = 0; i < NODES; i++) //hope element 256 has nothing in it... (skipped in this loop)
	{
		/*if (nodeList[i][0] != INFINITY) //print current ranking node to debug
		{
			fprintf(stderr, "\t\t\tRank of node %d: %d \n", nodeList[i][0], i);
			fflush(stderr);
		}*/

		if (nodeCurrent[NODEID] == INFINITY) //if current node is blank
		{
			//simLog("All nodes on stack checked.");
			return stack;
		}
		else if (nodeList[i][NODEID] == nodeCurrent[0]) //if current node has same ID
		{
			simLog("ERROR: Node already exists.");
			return stack;
		}
		else if (nodeCurrent[DIST] < nodeList[i][DIST]) //if current node is closer to start than node in ranking
		{
			if (!rankFound)
			{
				rankFound = true;
				stack = i;
			}

			//store information of node in ranking below current node to temporary array
			for (int j = 0; j < DATA; j++)
			{
				tempArr[j] = nodeList[i][j];
			}

			//add current node to the stack
			for (int j = 0; j < DATA; j++)
			{
				nodeList[i][j]=nodeCurrent[j];
			}

			//treat the node in temporary storage as the new current node
			for (int j = 0; j < DATA; j++)
			{
				nodeCurrent[j] = tempArr[j];
			}
			fprintf(stderr, "\t\t\tRank of node %d: %d \n", nodeList[i][0], i);
			fflush(stderr);
		}
	}
	simLog("ERROR: NO SPACE LEFT ON STACK");
	return stack;
}
