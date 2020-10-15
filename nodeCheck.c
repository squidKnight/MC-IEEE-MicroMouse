/*
Written by Mathazzar
Last modified: 10/15/20
Purpose: check to see if the current location is a node
Status: FINISHED, NOT TESTED
*/

#include "API.h" //only needed for simulator use
#include <stdbool.h>
#include <stdio.h>
#include "mouseDefs.h"
#include "nodeDefs.h"

/*bool nodeCheck(bool nodeCurrent[DATA])
INPUTS: bool nodeCurrent[DATA]
	nodeCurrent: the node whose state is to be determined
RETURNS: bool nodeCheck
	nodeCheck: if true, the node has been visited and therefore exists; if false, the node has never been visited and therefore doesn't exist yet
NOTES:
	The actual call, if based directly on nodeID instead of nodeCurrent, should be:
		nodeCheck(nodeList[nodeID - 1])
	Assumes that if there's a valid path / a direction without a wall, then the node has been visited to record some data and therefore exists.
CAUTION:
*/
bool nodeCheck(bool nodeCurrent[DATA])
{
	if (nodeCurrent[WAL_T] || nodeCurrent[WAL_R] || nodeCurrent[WAL_B] || nodeCurrent[WAL_L])
		return true;
	else
		return false;
}

/*short int node_(short int nodeCurrent)
INPUTS: short int nodeCurrent
	nodeCurrent: the position in the nodeList array of the refernce node
RETURNS: short int nodeCurrent
	nodeCurrent: performs translation to identify an adjacent node
NOTES:
	nodeID can be found by adding 1 to the output of any of this set of functions.
	nodeCurrent can be found by subtracting 1 from the nodeID of the node input into any of this set of functions.
CAUTION:
		Additional +/- 1 operation may be needed if nodeID is required and not the position in the array of the node.
*/
short int node(short int nodeCurrent)
{
	return nodeCurrent;
}
short int node_T(short int nodeCurrent)
{
	return nodeCurrent + 16;
}
short int node_R(short int nodeCurrent)
{
	return nodeCurrent + 1;
}
short int node_B(short int nodeCurrent)
{
	return nodeCurrent - 16;
}
short int node_L(short int nodeCurrent)
{
	return nodeCurrent - 1;
}
