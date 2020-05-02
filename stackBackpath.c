/*
Written by Mathazzar
Last modified: 05/2/20
Purpose: recursively check the backpath of nodeCurrent to update shortest route to start.
Status: FINISHED, NOT TESTED
*/

#include <stdbool.h>
#include <stdio.h>
#include "API.h" //only needed for simulator use
#include "mouseDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)
int stackCheck(int nodeList[NODES][DATA], int nodeCurrent); //adds new node into correct rank in stack based on distance
int stackInsert(int nodeList[NODES][DATA], int nodeCurrent[DATA]);
bool stackRemove(int nodeList[NODES][DATA], int rank);

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
CAUTION:
	Manipulates the nodeList array directly.
	Recursive function: will require sufficient memory to process all recursive steps.
	Walls, deadends and loopbacks are not currently checked for; not sure if they'll cause problems without checks to ignore them.
*/
int stackBackpath(int nodeList[NODES][DATA], int nodeID, int nodePrevious, int distLastNode)
{
	//base case
	if ((nodeList[stackCheck(nodeList, nodePrevious)][DIST] + distLastNode) >= nodeList[stackCheck(nodeList, nodeID)][DIST])
	{
		return stackCheck(nodeList, nodeID);
	}
	//recursive case:
	else if ((nodeList[stackCheck(nodeList, nodePrevious)][DIST] + distLastNode) < nodeList[stackCheck(nodeList, nodeID)][DIST])
	{
		int distPrevious = nodeList[stackCheck(nodeList, nodeID)][DIST];
		//update nodeCurrent
		nodeList[stackCheck(nodeList, nodeID)][DIST] = nodeList[stackCheck(nodeList, nodePrevious)][DIST] + distLastNode;
		nodeList[stackCheck(nodeList, nodeID)][NODEID_P] = nodePrevious;
		
		//check other directions
		int diff;
		if (nodeList[stackCheck(nodeList, nodeID)][NODEID_P] == nodeList[stackCheck(nodeList, nodeID)][NODEID_T]) //if new backpath is top
		{
			diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_R])][DIST]);
			stackBackpath(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_R], nodeID, diff);
			diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_B])][DIST]);
			stackBackpath(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_B],  nodeID, diff);
			diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_L])][DIST]);
			stackBackpath(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_L], nodeID, diff);
		}
		else if (nodeList[stackCheck(nodeList, nodeID)][NODEID_P] == nodeList[stackCheck(nodeList, nodeID)][NODEID_R]) //if new backpath is right
		{
			diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_T])][DIST]);
			stackBackpath(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_T], nodeID, diff);
			diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_B])][DIST]);
			stackBackpath(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_B], nodeID, diff);
			diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_L])][DIST]);
			stackBackpath(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_L], nodeID, diff);
		}
		else if (nodeList[stackCheck(nodeList, nodeID)][NODEID_P] == nodeList[stackCheck(nodeList, nodeID)][NODEID_B]) //if new backpath is bottom
		{
			diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_T])][DIST]);
			stackBackpath(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_T], nodeID, diff);
			diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_R])][DIST]);
			stackBackpath(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_R], nodeID, diff);
			diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_L])][DIST]);
			stackBackpath(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_L], nodeID, diff);
		}
		else if (nodeList[stackCheck(nodeList, nodeID)][NODEID_P] == nodeList[stackCheck(nodeList, nodeID)][NODEID_L]) //if new backpath is left
		{
			diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_T])][DIST]);
			stackBackpath(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_T], nodeID, diff);
			diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_R])][DIST]);
			stackBackpath(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_R], nodeID, diff);
			diff = abs(distPrevious - nodeList[stackCheck(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_B])][DIST]);
			stackBackpath(nodeList, nodeList[stackCheck(nodeList, nodeID)][NODEID_B], nodeID, diff);
		}
		else
		{
			simLog("ERROR: backpath is not equal to any recorded direction.");
			return stackCheck(nodeList, nodeID);
		}

		//sort nodeList
		int rank = stackCheck(nodeList, nodeID);
		int stack = stackInsert(nodeList, nodeList[rank]);
		stackRemove(nodeList, rank);
		return stack;
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