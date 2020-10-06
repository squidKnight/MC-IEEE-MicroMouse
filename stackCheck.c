/*
Written by Mathazzar
Last modified: 10/06/20
Purpose: check if the current node is already on the stack
Status: FINISHED, TESTED
*/

#include <stdbool.h>
#include <stdio.h>
#include "API.h" //only needed for simulator use
#include "mouseDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)

/*int stackCheck(int nodeList[NODES][DATA], int nodeCurrent)
INPUTS: int nodeList[NODES][DATA], int nodeCurrent
	nodeList: nodeList array
	nodeCurrent: NODEID of nodeCurrent array
RETURNS: int rankFound
	rankFound: the current rank of the node on the stack. If it's not on the stack yet, it will return INFINITY
*/
short int stackCheck(short int nodeList[NODES][DATA], short int nodeCurrent) //checks new node for int it's already in the stack based on nodeID
{
	//simLog("\t\tChecking if node is already on stack...");
	if (nodeCurrent == INFINITY) //if current node is blank
	{
		simLog("ERROR: Blank Node");
		return INFINITY;
	}
	short int rankFound = INFINITY; //becomes rank of node on stack if new node is found already on the stack
	for (int i = 0; i < 256; i++) //hope element 256 has nothing in it... (skipped in this loop)
	{
		/*
		if (nodeList[i][0] != INFINITY) //print current ranking node to debug
		{
		fprintf(stderr, "\t\t\tRank of node %d: %d \n", nodeList[i][0], i);
		fflush(stderr);
		}
		*/
		
		if (nodeList[i][0] == nodeCurrent) //if current node has same ID
		{
			//simLog("Node already exists.");
			rankFound = i;
		}
	}
	return rankFound;
}
