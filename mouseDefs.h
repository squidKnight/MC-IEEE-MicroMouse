/*
Written by Mathazzar
Last modified: 06/04/20
Purpose: store universal constants for data.
Status: FINISHED, TESTED
*/

#pragma once

#define INFINITY 1024 //interpreted as null value for stored integers

//nodeList array definitions
#define NODES 256//number of possible nodes that can be stored in the nodeList array
#define DATA 9//number of data-points in the nodeList array
/*
	[0]nodeID, [1]abbsolute distance from start(sorting method), [2]backpath nodeID, 
	path nodeIDs(INFINITY=WALL, unexplored=0):
		[3]top path nodeID, [4]right path nodeID, [5]bottom path nodeID, [6]left path nodeID,
	fully explored bools(0=false,1=true):
		[7]top path, [8]right path, [9]bottom path,[10]left path
*/

//data-points for easy absolute clarification
#define NODEID 0
#define NODEID_T 1
#define NODEID_R 2
#define NODEID_B 3
#define NODEID_L 4
#define DIST_T 5
#define DIST_R 6
#define DIST_B 7
#define DIST_L 8
