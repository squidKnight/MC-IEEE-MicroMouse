/*
Written by Mathazzar
Last modified: 10/14/20
Purpose: check to see if the current location is a node
Status: UNFINISHED, NOT TESTED
*/

#include "API.h" //only needed for simulator use
#include <stdbool.h>
#include <stdio.h>
#include "mouseDefs.h"

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