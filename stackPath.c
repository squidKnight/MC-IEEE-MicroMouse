/*
Written by Mathazzar
Last modified: 10/15/20
Purpose: find the shortest path to a given node from a origin node in the nodeList array using Dijkstra's algorithm.
Status: FINISHED, NOT TESTED
*/

#include <stdbool.h>
#include <stdio.h>
#include "API.h" //only needed for simulator use
#include "mouseDefs.h"
#include "nodeDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)

static bool nodesExistAlt(short int list[NODES / 8], short int nodeID);

/*void stackPath(bool nodeList[NODES][DATA], short int pathList[NODES / 8], short int holdList[NODES], short int nodeCurrent, short int nodeNext)
INPUTS: bool nodeList[NODES][DATA], short int pathList[NODES / 8], short int holdList[NODES], short int nodeCurrent, short int nodeNext
	nodeList: the nodeList array.
	pathList: blank array to be filled by stackPath() listing the order of each node to travel to from nodeCurrent to nodeNext.
	holdList: lists the distances of each node from nodeCurrent. Minimum Spanning Tree.
	nodeCurrent: nodeID of the current node on the stack that the micromouse is at and must calculate the minimum spanning tree for.
	nodeNext: the next node to be traveled to.
RETURNS: bool nodeList[NODES][DATA], short int pathList[NODES / 8], short int holdList[NODES]
	nodeList: should return unmodified.
	pathList: directly modified by stackPath(), lists the order of nodes to take to get from nodeCurrent to nodeNext.
	holdList: should return unmodified.
NOTES:
	Implemented to have pathList and holdList arrays created externally beforehand, and assumes them to be blank and initializes them.
	Currently assumes that any path possible will be less than NODES/8(~32) long.
CAUTION:
	Interacts with the nodeList array passed to it directly.
	Interacts with the holdList array passed to it directly.
	Manipulates the pathList array passed to it directly.
*/
void stackPath(bool nodeList[NODES][DATA], short int pathList[NODES / 8], short int holdList[NODES], short int nodeCurrent, short int nodeNext)
{
	simLog("Calculating shortest path...");
	//initialize data-set
	for (int i = 0; i < NODES / 8; i++)
	{
		pathList[i] = INFINITY;
	}

	//calculate backpath from nextNode to nodeCurrent
	short int nodeID = nodeNext;
	short int reverseList[NODES / 8];
	for (int i = 0; i < NODES / 8; i++)
		reverseList[i] = INFINITY;
	reverseList[0] = nodeID;
	short int x = 0;
	simLog("Generating Path...");
	while (nodeID != nodeCurrent)
	{
		short int minDist = INFINITY;
		short int vert = nodeID - 1;
		if (!nodeCheck(nodeList[vert]))
			return;
		short int stick = node_T(vert);
		if (nodeCheck(nodeList[stick]) && (holdList[stick] < minDist))
		{
			minDist = holdList[stick];
			nodeID = stick + 1;
		}
		stick = node_R(vert);
		if (nodeCheck(nodeList[stick]) && (holdList[stick] < minDist))
		{
			minDist = holdList[stick];
			nodeID = stick + 1;
		}
		stick = node_B(vert);
		if (nodeCheck(nodeList[stick]) && (holdList[stick] < minDist))
		{
			minDist = holdList[stick];
			nodeID = stick + 1;
		}
		stick = node_L(vert);
		if (nodeCheck(nodeList[stick]) && (holdList[stick] < minDist))
		{
			minDist = holdList[stick];
			nodeID = stick + 1;
		}

		if (nodesExistAlt(reverseList, nodeID))
			simLog("CRITICAL ERROR: node already on list, loop may occur.");
		reverseList[++x] = nodeID;
	}
	for (int i = 0; i < NODES; i++)
	{
		pathList[i] = reverseList[x--];
		if (x < 0)
			break;
	}

	simLog("Optimal Path Generated:");
	for (int i = 0; i < NODES / 8; i++)
	{
		fprintf(stderr, "\t\t\tnodeID: %d\n", pathList[i]);
		fflush(stderr);
	}
}

static bool nodesExistAlt(short int list[NODES / 8], short int nodeID)
{
	for (int i = 0; i < NODES / 8; i++)
	{
		if (list[i] == nodeID)
			return true;
	}
	return false;
}
