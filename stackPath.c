/*
Written by Mathazzar
Last modified: 06/04/20
Purpose: find the shortest path to a given node from a origin node in the nodeList array using Dijkstra's algorithm.
Status: NOT FINISHED, NOT TESTED
*/

#include <stdbool.h>
#include <stdio.h>
#include "API.h" //only needed for simulator use
#include "mouseDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)
int stackCheck(int nodeList[NODES][DATA], int nodeCurrent); //adds new node into correct rank in stack based on distance
short int directionNext(int nodeCurrent[DATA], int nodeNext);
void pathTree(int nodeList[NODES][DATA], int holdList[NODES], int nodeCurrent);

static bool nodesExistAlt(int list[NODES / 8], int nodeID);

/*void stackPath(int nodeList[NODES][DATA], int pathList[NODES / 8], int holdList[NODES], int nodeCurrent, int nodeNext)
INPUTS: int nodeList[NODES][DATA], int pathList[NODES / 8], int holdList[NODES], int nodeCurrent, int nodeNext
	nodeList: the nodeList array.
	pathList: blank array to be filled by stackPath() listing the order of each node to travel to from nodeCurrent to nodeNext.
	holdList: lists the distances of each node from nodeCurrent.
	nodeCurrent: nodeID of the current node on the stack that the micromouse is at and must calculate the minimum spanning tree for.
	nodeNext: the next node to be traveled to.
RETURNS: int nodeList[NODES][DATA], int pathList[NODES / 8], int holdList[NODES]
	nodeList: should return unmodified.
	pathList: directly modified by stackPath(), lists the order of nodes to take to get from nodeCurrent to nodeNext.
	holdList: should return unmodified.
NOTES:
	Implemented to have pathList and holdList arrays created externally beforehand, and assumes them to be blank and initializes them.
	Currently assumes that any path possible will be less than NODES/8(~32) long.
CAUTION:
	Interacts with the nodeList array passed to it directly.
	Interacts with the holdList array passed to it directly.
	manipulates the pathList array passed to it directly.
*/
void stackPath(int nodeList[NODES][DATA], int pathList[NODES / 8], int holdList[NODES], int nodeCurrent, int nodeNext)
{
	simLog("Calculating shortest path...");
	//initialize data-set
	//int holdList[NODES]; //distance from nodeCurrent of each node
	for (int i = 0; i < NODES / 8; i++)
	{
		pathList[i] = INFINITY;
	}
	//holdList[stackCheck(nodeList, nodeCurrent)] = 0;

	//generate minimum spanning tree
	//pathTree(nodeList, holdList, nodeCurrent);

	//calculate backpath from nextNode to nodeCurrent
	int nodeID = nodeNext;
	int reverseList[NODES / 8];
	for (int i = 0; i < NODES / 8; i++)
		reverseList[i] = INFINITY;
	reverseList[0] = nodeID;
	int x = 0;
	simLog("Generating Path...");
	while (nodeID != nodeCurrent)
	{
		int minDist = INFINITY;
		int vert = stackCheck(nodeList, nodeID);
		if (vert == INFINITY)
			return;
		int stick = stackCheck(nodeList, nodeList[vert][NODEID_T]);
		if ((stick != INFINITY) && (holdList[stick] < minDist))
		{
			minDist = holdList[stick];
			nodeID = nodeList[stick][NODEID];
		}
		stick = stackCheck(nodeList, nodeList[vert][NODEID_R]);
		if ((stick != INFINITY) && (holdList[stick] < minDist))
		{
			minDist = holdList[stick];
			nodeID = nodeList[stick][NODEID];
		}
		stick = stackCheck(nodeList, nodeList[vert][NODEID_B]);
		if ((stick != INFINITY) && (holdList[stick] < minDist))
		{
			minDist = holdList[stick];
			nodeID = nodeList[stick][NODEID];
		}
		stick = stackCheck(nodeList, nodeList[vert][NODEID_L]);
		if ((stick != INFINITY) && (holdList[stick] < minDist))
		{
			minDist = holdList[stick];
			nodeID = nodeList[stick][NODEID];
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

static bool nodesExistAlt(int list[NODES / 8], int nodeID)
{
	for (int i = 0; i < NODES / 8; i++)
	{
		if (list[i] == nodeID)
			return true;
	}
	return false;
}