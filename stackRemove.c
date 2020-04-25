/*
Written by Mathazzar
Last modified: 04/25/20
Purpose: remove the specifiend ranking node from the stack
*/

#include <stdbool.h>
#include <stdio.h>
#include "mouseDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)

/*bool stackRemove(int nodeList[NODES][DATA], int rank)
INPUTS: int nodeList[NODES][DATA], int rank
	nodeList: the nodeList array
	rank: the rank of the node in the array to be removed
RETURNS: true
NOTES:
	Primary function to interact with the nodeList array.
CAUTION:
	Manipulates the nodeList array directly.
	Permanently removes data without an internal save. To preserve information, data must be stored externally first.
*/
bool stackRemove(int nodeList[NODES][DATA], int rank)
{
	simLog("\t\tRemoving node from stack...");
	bool rankFound = 0; //becomes 1 when stack rank for new node is found
	for (int i = rank; i < NODES; i++) //hope element 256 has nothing in it... (skipped in this loop)
	{
		//store information of node in ranking above current node to current node in array
		for (int j = 0; j < DATA; j++)
		{
			nodeList[i][j] = nodeList[i+1][j];
		}
		fprintf(stderr, "\t\t\tRank of node %d: %d \n", nodeList[i][0], i);
		fflush(stderr);
	}
	simLog("\t\tNode removed from stack.");
	return true;
}