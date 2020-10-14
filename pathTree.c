/*
Written by Mathazzar
Last modified: 10/14/20
Purpose: generate a minimum spanning tree from a given node to all other nodes.
Status: FINISHED, NOT TESTED
*/

#include <stdbool.h>
#include <stdio.h>
#include "API.h" //only needed for simulator use
#include "mouseDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)
bool nodeCheck(bool nodeCurrent[DATA]); //checks to see if the current location is a node

static bool nodesExist(bool hold[NODES]);
static short int node(short int nodeID);
static short int node_T(short int nodeID);
static short int node_R(short int nodeID);
static short int node_B(short int nodeID);
static short int node_L(short int nodeID);

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
	Currently gets stuck in a loop if the start node is a node(if it's a corner and not a deadend, traveling to it from any other node
		will cause the micromouse to pass over it and continue to the opposite node connected to it and link them together instead).
		Need to test if solved with new implementation.
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
	if (!nodeCheck(nodeCurrent - 1))
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
		simLog("top");
		short int stackCurrent = node_T(minStack); //check top
		if (nodeCheck(stackCurrent) && !hold[stackCurrent] && (nodeList[minStack][WAL_T] && nodeList[stackCurrent][WAL_B]))
			holdList[stackCurrent] = holdList[minStack] + 1;
		simLog("right");
		stackCurrent = node_R(minStack); //check right
		if (nodeCheck(stackCurrent) && !hold[stackCurrent] && (nodeList[minStack][WAL_R] && nodeList[stackCurrent][WAL_L]))
			holdList[stackCurrent] = holdList[minStack] + 1;
		simLog("bottom");
		stackCurrent = node_B(minStack); //check bottom
		if (nodeCheck(stackCurrent) && !hold[stackCurrent] && (nodeList[minStack][WAL_B] && nodeList[stackCurrent][WAL_T]))
			holdList[stackCurrent] = holdList[minStack] + 1;
		simLog("left");
		stackCurrent = node_L(minStack); //check left
		if (nodeCheck(stackCurrent) && !hold[stackCurrent] && (nodeList[minStack][WAL_L] && nodeList[stackCurrent][WAL_R]))
			holdList[stackCurrent] = holdList[minStack] + 1;
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

static short int node(short int nodeID)
{
	return nodeID;
}

static short int node_T(short int nodeID)
{
	return nodeID + 16;
}

static short int node_R(short int nodeID)
{
	return nodeID + 1;
}

static short int node_B(short int nodeID)
{
	return nodeID - 16;
}

static short int node_L(short int nodeID)
{
	return nodeID - 1;
}
