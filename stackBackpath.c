/*
Written by Mathazzar
Last modified: 05/24/20
Purpose: recursively check the backpath of nodeCurrent to update shortest route to start.
Status: FINISHED, TESTED
*/

#include <stdbool.h>
#include <stdio.h>
//#include <stdlib.h>
#include "API.h" //only needed for simulator use
#include "mouseDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)
int stackCheck(int nodeList[NODES][DATA], int nodeCurrent); //adds new node into correct rank in stack based on distance
//int stackInsert(int nodeList[NODES][DATA], int nodeCurrent[DATA]);
//int stackRemove(int nodeList[NODES][DATA], int rank);

static int abs(int x);

/*int stackBackpath(int nodeList[NODES][DATA], int nodeID, int nodePrevious, int distLastNode)
INPUTS: int nodeList[NODES][DATA], int nodeID, int nodePrevious, int distLastNode
	nodeList: the nodeList array.
	nodeID: ID of current node.
	nodePrevious: current updated path's latest node.
	distLastNode: distance between current node and nodePrevious.
RETURNS: stackCheck(nodeList, nodeID), nodeList
	stack: the position on the stack of the current node; nodeID.
	nodeList: updates the backpaths of nodes and their position in the array directly.
NOTES:
	Implemented to be called in scan.c as:
		rank = stackBackpath(nodeList, nodeID, nodePrevious, distLastNode);
		to set rank to the new position in the array the current node is listed at.
	Assumes nodeID and nodePrevious have direct connections to eachother.
	It's recursively calling an array simulating a quadrupley-linked-list; expect things to be misconfigured...
	Loopbacks not directly checked for, relies upon the path already being updated and shorter.
CAUTION:
	Manipulates the nodeList array directly.
	Recursive function: will require sufficient memory to process all recursive steps.
*/
int stackBackpath(int nodeList[NODES][DATA], int nodeID, int nodePrevious, int distLastNode)
{
	fprintf(stderr, "\tChecking NodeID %d...\n", nodeID);
	fflush(stderr);
	int rankID = stackCheck(nodeList, nodeID);
	int rankPr = stackCheck(nodeList, nodePrevious);
	//base case
	if ((nodeList[rankPr][DIST] + distLastNode) >= nodeList[rankID][DIST])
	{
		fprintf(stderr, "\t\tNodeID %d is fine.\n", nodeID);
		fflush(stderr);
		return rankID;
	}
	//recursive case:
	else if ((nodeList[rankPr][DIST] + distLastNode) < nodeList[rankID][DIST])
	{
		int distPrevious = nodeList[rankID][DIST];
		//update nodeCurrent
		nodeList[rankID][DIST] = nodeList[rankPr][DIST] + distLastNode;
		nodeList[rankID][NODEID_P] = nodePrevious;
		
		//check other directions
		int diff;
		if ((nodeList[rankID][NODEID_P] == nodeList[rankID][NODEID_T])) //if new backpath is top
		{
			if ((nodeList[rankID][NODEID_R] != 0) && (nodeList[rankID][NODEID_R] != INFINITY))
			{
				simLog("\t\tChecking right...0");
				diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[rankID][NODEID_R])][DIST]);
				stackBackpath(nodeList, nodeList[rankID][NODEID_R], nodeID, diff);
			}
			if ((nodeList[rankID][NODEID_B] != 0) && (nodeList[rankID][NODEID_B] != INFINITY))
			{
				simLog("\t\tChecking down...0");
				diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[rankID][NODEID_B])][DIST]);
				stackBackpath(nodeList, nodeList[rankID][NODEID_B], nodeID, diff);
			}
			if ((nodeList[rankID][NODEID_L] != 0) && (nodeList[rankID][NODEID_L] != INFINITY))
			{
				simLog("\t\tChecking left...0");
				diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[rankID][NODEID_L])][DIST]);
				stackBackpath(nodeList, nodeList[rankID][NODEID_L], nodeID, diff);
			}
		}
		else if ((nodeList[rankID][NODEID_P] == nodeList[rankID][NODEID_R])) //if new backpath is right
		{
			if ((nodeList[rankID][NODEID_T] != 0) && (nodeList[rankID][NODEID_T] != INFINITY))
			{
				simLog("\t\tChecking up...1");
				diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[rankID][NODEID_T])][DIST]);
				stackBackpath(nodeList, nodeList[rankID][NODEID_T], nodeID, diff);
			}
			if ((nodeList[rankID][NODEID_B] != 0) && (nodeList[rankID][NODEID_B] != INFINITY))
			{
				simLog("\t\tChecking down...1");
				diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[rankID][NODEID_B])][DIST]);
				stackBackpath(nodeList, nodeList[rankID][NODEID_B], nodeID, diff);
			}
			if ((nodeList[rankID][NODEID_L] != 0) && (nodeList[rankID][NODEID_L] != INFINITY))
			{
				simLog("\t\tChecking left...1");
				diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[rankID][NODEID_L])][DIST]);
				stackBackpath(nodeList, nodeList[rankID][NODEID_L], nodeID, diff);
			}
		}
		else if ((nodeList[rankID][NODEID_P] == nodeList[rankID][NODEID_B])) //if new backpath is bottom
		{
			if ((nodeList[rankID][NODEID_T] != 0) && (nodeList[rankID][NODEID_T] != INFINITY))
			{
				simLog("\t\tChecking up...2");
				diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[rankID][NODEID_T])][DIST]);
				stackBackpath(nodeList, nodeList[rankID][NODEID_T], nodeID, diff);
			}
			if ((nodeList[rankID][NODEID_R] != 0) && (nodeList[rankID][NODEID_R] != INFINITY))
			{
				simLog("\t\tChecking right...2");
				diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[rankID][NODEID_R])][DIST]);
				stackBackpath(nodeList, nodeList[rankID][NODEID_R], nodeID, diff);
			}
			if ((nodeList[rankID][NODEID_L] != 0) && (nodeList[rankID][NODEID_L] != INFINITY))
			{
				simLog("\t\tChecking left...2");
				diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[rankID][NODEID_L])][DIST]);
				stackBackpath(nodeList, nodeList[rankID][NODEID_L], nodeID, diff);
			}
		}
		else if ((nodeList[rankID][NODEID_P] == nodeList[rankID][NODEID_L])) //if new backpath is left
		{
			if ((nodeList[rankID][NODEID_T] != 0) && (nodeList[rankID][NODEID_T] != INFINITY))
			{
				simLog("\t\tChecking up...3");
				diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[rankID][NODEID_T])][DIST]);
				stackBackpath(nodeList, nodeList[rankID][NODEID_T], nodeID, diff);
			}
			if ((nodeList[rankID][NODEID_R] != 0) && (nodeList[rankID][NODEID_R] != INFINITY))
			{
				simLog("\t\tChecking right...3");
				diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[rankID][NODEID_R])][DIST]);
				stackBackpath(nodeList, nodeList[rankID][NODEID_R], nodeID, diff);
			}
			if ((nodeList[rankID][NODEID_B] != 0) && (nodeList[rankID][NODEID_B] != INFINITY))
			{
				simLog("\t\tChecking down...3");
				diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[rankID][NODEID_B])][DIST]);
				stackBackpath(nodeList, nodeList[rankID][NODEID_B], nodeID, diff);
			}
		}
		else
		{
			simLog("ERROR: backpath is not equal to any recorded direction.");
			return rankID;
		}

		fprintf(stderr, "\tNodeID %d has been recalculated.\n", nodeID);
		fflush(stderr);
		return rankID;
	}
	else
	{
		simLog("RECURSIVE ERROR");
		return stackCheck(nodeList, nodeID);
	}
}

/*
//rank: rank of current node(stackCheck(nodeList, nodeID))
//stack: rank of node previous(stackCheck(nodeList, nodePrevious))
//nodeID: ID of current node(nodeList[rank][NODEID])
//nodePrevious: current updated path's latest node(nodeList[stack][NODEID])
//distLastNode: distance from nodePrevious([rank][DIST]-[stack][DIST])
//diff: distance between current node and nodePrevious. Needs to be checked before nodePrevious's dist is updated

base case: current node is closer to start than updated path
	if nodePrevious dist+diff >= nodeCurrent dist
		return;
recursive case: current node is further from start than updated path
	else if nodePrevious dist+diff < nodeID dist
		distPrevious = nodeCurrent dist;
		//update nodeCurrent
			nodeCurrent dist = nodePrevious dist + diff
			nodeCurrent backpath = nodePrevious
		//check other directions
			if (nodeCurrent backpath == nodeCurrent top)
				diff = |distPrevious - nodeList[stackCheck(nodeCurrent right)] dist|
				stackBackpath(nodeList, stackCheck(nodeCurrent right), rank, nodeCurrent right, diff), nodeCurrent dist
				diff = |distPrevious - nodeList[stackCheck(nodeCurrent bottom)] dist|
				stackBackpath(nodeList, stackCheck(nodeCurrent bottom), rank, nodeCurrent bottom, diff), nodeCurrent dist
				diff = |distPrevious - nodeList[stackCheck(nodeCurrent left)] dist|
				stackBackpath(nodeList, stackCheck(nodeCurrent left), rank, nodeCurrent left, diff), nodeCurrent dist
			else if (nodeCurrent backpath == nodeCurrent right)
				diff = |distPrevious - nodeList[stackCheck(nodeCurrent top)] dist|
				stackBackpath(nodeList, stackCheck(nodeCurrent top), rank, nodeCurrent top, diff), nodeCurrent dist
				diff = |distPrevious - nodeList[stackCheck(nodeCurrent bottom)] dist|
				stackBackpath(nodeList, stackCheck(nodeCurrent bottom), rank, nodeCurrent bottom, diff), nodeCurrent dist
				diff = |distPrevious - nodeList[stackCheck(nodeCurrent left)] dist|
				stackBackpath(nodeList, stackCheck(nodeCurrent left), rank, nodeCurrent left, diff), nodeCurrent dist
			else if (nodeCurrent backpath == nodeCurrent bottom)
				diff = |distPrevious - nodeList[stackCheck(nodeCurrent top)] dist|
				stackBackpath(nodeList, stackCheck(nodeCurrent top), rank, nodeCurrent top, diff), nodeCurrent dist
				diff = |distPrevious - nodeList[stackCheck(nodeCurrent right)] dist|
				stackBackpath(nodeList, stackCheck(nodeCurrent right), rank, nodeCurrent right, diff), nodeCurrent dist
				diff = |distPrevious - nodeList[stackCheck(nodeCurrent left)] dist|
				stackBackpath(nodeList, stackCheck(nodeCurrent left), rank, nodeCurrent left, diff), nodeCurrent dist
			else if (nodeCurrent backpath == nodeCurrent left)
				diff = |distPrevious - nodeList[stackCheck(nodeCurrent top)] dist|
				stackBackpath(nodeList, stackCheck(nodeCurrent top), rank, nodeCurrent top, diff), nodeCurrent dist
				diff = |distPrevious - nodeList[stackCheck(nodeCurrent right)] dist|
				stackBackpath(nodeList, stackCheck(nodeCurrent right), rank, nodeCurrent right, diff), nodeCurrent dist
				diff = |distPrevious - nodeList[stackCheck(nodeCurrent bottom)] dist|
				stackBackpath(nodeList, stackCheck(nodeCurrent bottom), rank, nodeCurrent bottom, diff), nodeCurrent dist
		return;
	else
		ERROR: not possible for nodePrevious dist to neither be >= or < nodeCurrent dist
*/

static int abs(int x)
{
	if (x < 0)
		return x*-1;
	else
		return x;
}
