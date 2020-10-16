/*
Written by Mathazzar
Last modified: 10/15/20
Purpose: generate a minimum spanning tree from a given node to all other nodes.
Status: FINISHED, TESTED
*/

#include <stdbool.h>
#include <stdio.h>
#include "API.h" //only needed for simulator use
#include "mouseDefs.h"
#include "nodeDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)

static bool nodesExist(bool hold[NODES]);

/*void pathTree(bool nodeList[NODES][DATA], short int holdList[NODES], short int nodeCurrent)
INPUTS: short int nodeList[NODES][DATA], short int holdList[NODES], short int nodeCurrent
	nodeList: the nodeList array.
	holdList: blank array to be filled by pathTree() listing the distances of each node from nodeCurrent.
	nodeCurrent: nodeID of the current node on the stack that the micromouse is at and must calculate the minimum spanning tree for.
RETURNS: bool nodeList[NODES][DATA], short int holdList[NODES]
	nodeList: should return unmodified.
	holdList: directly modified by pathTree(), lists the distances of each node from nodeCurrent.
NOTES:
	Implemented to have holdList array created externally beforehand, and assumes it to be blank and initializes it.
	Manipulates nodeCurrent directly, though it's immutable and won't be affected when this fuction exits.
CAUTION:
	Interacts with the nodeList array passed to it directly.
	Manipulates the holdList array passed to it directly.
*/
void pathTree(bool nodeList[NODES][DATA], short int holdList[NODES], short int nodeCurrent)
{
	simLog("Generating Minimum Spanning Tree...");
	//initialize data-set
	bool hold[NODES]; //finalized nodes
	for (int i = 0; i < NODES; i++)
	{
		holdList[i] = INFINITY;
		if (nodeCheck(nodeList[i])) //if it's a node, it shouldn't hold
			hold[i] = false;
		else
			hold[i] = true;
	}
	if (!nodeCheck(nodeList[nodeCurrent - 1]))
	{
		simLog("CRITICAL ERROR: nodeCurrent can't be found on stack.");
		return;
	}
	else
		holdList[nodeCurrent - 1] = 0;
	for (int i = 0; i < NODES; i++)
	{
		fprintf(stderr, "\t\t\tnodeID: %d, holdList: %d, hold: %d\n", i + 1, holdList[i], hold[i]);
		fflush(stderr);
	}

	//generate minimum spanning tree
	while (nodesExist(hold))
	{
		//find next node with shortest distance
		short int minDist = INFINITY;
		short int minID = INFINITY;
		short int minStack;
		for (int i = 0; i < NODES; i++)
		{
			if (!hold[i] && (holdList[i] < minDist))
			{
				minDist = holdList[i];
				minID = i + 1;
				minStack = i;
			}
		}
		hold[minStack] = true;
		fprintf(stderr, "Checking nodeID %d's paths...\n", minID);
		fflush(stderr);

		//calculated distances for adjacent nodes
		simLog("\ttop");
		short int stackCurrent = node_T(minStack); //check top
		if (nodeCheck(nodeList[stackCurrent]) && !hold[stackCurrent] && (nodeList[minStack][WAL_T] && nodeList[stackCurrent][WAL_B]))
			holdList[stackCurrent] = holdList[minStack] + 1;
		else
			simLog("\t\tnope");
		simLog("\tright");
		stackCurrent = node_R(minStack); //check right
		if (nodeCheck(nodeList[stackCurrent]) && !hold[stackCurrent] && (nodeList[minStack][WAL_R] && nodeList[stackCurrent][WAL_L]))
			holdList[stackCurrent] = holdList[minStack] + 1;
		else
			simLog("\t\tnope");
		simLog("\tbottom");
		stackCurrent = node_B(minStack); //check bottom
		if (nodeCheck(nodeList[stackCurrent]) && !hold[stackCurrent] && (nodeList[minStack][WAL_B] && nodeList[stackCurrent][WAL_T]))
			holdList[stackCurrent] = holdList[minStack] + 1;
		else
			simLog("\t\tnope");
		simLog("\tleft");
		stackCurrent = node_L(minStack); //check left
		if (nodeCheck(nodeList[stackCurrent]) && !hold[stackCurrent] && (nodeList[minStack][WAL_L] && nodeList[stackCurrent][WAL_R]))
			holdList[stackCurrent] = holdList[minStack] + 1;
		else
			simLog("\t\tnope");
	}
	simLog("Minimum Spanning Tree Generated:");
	for (int i = 0; i < NODES; i++)
	{
		fprintf(stderr, "\t\t\tnodeID: %d, holdList: %d, hold: %d\n", i + 1, holdList[i], hold[i]);
		fflush(stderr);
	}
}

static bool nodesExist(bool hold[NODES])
{
	for (int i = 0; i < NODES; i++)
	{
		if (!hold[i])
		{
			fprintf(stderr, "nodeID %d exists\n", i + 1);
			fflush(stderr);
			return true;
		}
	}
	return false;
}
