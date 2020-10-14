/*
Written by Mathazzar
Last modified: 10/06/20
Purpose: generate a minimum spanning tree from a given node to all other nodes.
Status: UNFINISHED, NOT TESTED
*/

#include <stdbool.h>
#include <stdio.h>
#include "API.h" //only needed for simulator use
#include "mouseDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)
short int stackCheck(short int nodeList[NODES][DATA], short int nodeCurrent); //find rank of nodeCurrent if it exists in nodeList
short int directionNext(short int nodeCurrent[DATA], short int nodeNext); //identifies the direction of the current node's backpath to prevent longer path to the next node being taken

static bool nodesExist(bool hold[NODES]);

/*void pathTree(short int nodeList[NODES][DATA], short int holdList[NODES], short int nodeCurrent)
INPUTS: short int nodeList[NODES][DATA], short int holdList[NODES], short int nodeCurrent
	nodeList: the nodeList array.
	holdList: blank array to be filled by pathTree() listing the distances of each node from nodeCurrent.
	nodeCurrent: nodeID of the current node on the stack that the micromouse is at and must calculate the minimum spanning tree for.
RETURNS: short int nodeList[NODES][DATA], short int holdList[NODES]
	nodeList: should return unmodified.
	holdList: directly modified by pathTree(), lists the distances of each node from nodeCurrent.
NOTES:
	Implemented to have holdList array created externally beforehand, and assumes it to be blank and initializes it.
	Manipulates nodeCurrent directly, though it's immutable and won't be affected when this fuction exits.
CAUTION:
	Interacts with the nodeList array passed to it directly.
	Manipulates the holdList array passed to it directly.
	Currently gets stuck in a loop if the start node is a node(if it's a corner and not a deadend, traveling to it from any other node
		will cause the micromouse to pass over it and continue to the opposite node connected to it and link them together instead).
*/
void pathTree(short int nodeList[NODES][DATA], short int holdList[NODES], short int nodeCurrent)
{
	simLog("Generating Minimum Spanning Tree...");
	//initialize data-set
	bool hold[NODES]; //finalized nodes
	for (int i = 0; i < NODES; i++)
	{
		holdList[i] = INFINITY;
		if (nodeList[i][NODEID] != INFINITY)
			hold[i] = false;
		else
			hold[i] = true;
	}
	short int vert = stackCheck(nodeList, nodeCurrent);
	if (vert == INFINITY)
	{
		simLog("CRITICAL ERROR: nodeCurrent can't be found on stack.");
		return;
	}
	else
		holdList[vert] = 0;
	for (int i = 0; i < NODES; i++)
	{
		fprintf(stderr, "\t\t\tnodeID: %d, holdList: %d, hold: %d\n", nodeList[i][NODEID], holdList[i], hold[i]);
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
				minID = nodeList[i][NODEID];
				minStack = i;
			}
		}
		hold[minStack] = true;
		fprintf(stderr, "Checking nodeID %d's paths...\n", minID);
		fflush(stderr);

		//calculated distances for adjacent nodes
		simLog("top");
		short int stackCurrent = stackCheck(nodeList, nodeList[minStack][NODEID_T]); //check top
		if ((stackCurrent != INFINITY) && !hold[stackCurrent] && (nodeList[minStack][NODEID_T] != 0))
			if ((minDist + nodeList[minStack][DIST_T]) < holdList[stackCurrent])
				holdList[stackCurrent] = nodeList[minStack][DIST_T] + minDist;
		simLog("right");
		stackCurrent = stackCheck(nodeList, nodeList[minStack][NODEID_R]); //check right
		if ((stackCurrent != INFINITY) && !hold[stackCurrent] && (nodeList[minStack][NODEID_R] != 0))
			if ((minDist + nodeList[minStack][DIST_R]) < holdList[stackCurrent])
				holdList[stackCurrent] = nodeList[minStack][DIST_R] + minDist;
		simLog("bottom");
		stackCurrent = stackCheck(nodeList, nodeList[minStack][NODEID_B]); //check bottom
		if ((stackCurrent != INFINITY) && !hold[stackCurrent] && (nodeList[minStack][NODEID_B] != 0))
			if ((minDist + nodeList[minStack][DIST_B]) < holdList[stackCurrent])
				holdList[stackCurrent] = nodeList[minStack][DIST_B] + minDist;
		simLog("left");
		stackCurrent = stackCheck(nodeList, nodeList[minStack][NODEID_L]); //check left
		if ((stackCurrent != INFINITY) && !hold[stackCurrent] && (nodeList[minStack][NODEID_L] != 0))
			if ((minDist + nodeList[minStack][DIST_L]) < holdList[stackCurrent])
				holdList[stackCurrent] = nodeList[minStack][DIST_L] + minDist;
	}
	simLog("Minimum Spanning Tree Generated:");
	for (int i = 0; i < NODES; i++)
	{
		fprintf(stderr, "\t\t\tnodeID: %d, holdList: %d, hold: %d\n", nodeList[i][NODEID], holdList[i], hold[i]);
		fflush(stderr);
	}
}

static bool nodesExist(bool hold[NODES])
{
	for (int i = 0; i < NODES; i++)
	{
		if (!hold[i])
		{
			fprintf(stderr, "%d exists\n", i);
			fflush(stderr);
			return true;
		}
	}
	return false;
}