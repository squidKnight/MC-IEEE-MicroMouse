/*
Written by Mathazzar
Last modified: 10/13/20
Purpose: store universal constants for data.
Status: FINISHED, NOT TESTED
*/

#pragma once

#include <stdbool.h>

#define INFINITY 1024 //interpreted as null value for stored integers

//nodeList array definitions
#define NODES 256//number of possible nodes that can be stored in the nodeList array
#define DATA 8//number of data-points in the nodeList array

/*nodeList[NODES][DATA]
nodeID - 1 = nodeList[i]
if path top: nodeID_T = i + 16 (nodeID + 15)
if path right: nodeID_R = i + 1 (nodeID)
if path bottom: nodeID_B = i - 16 (nodeID - 17)
if path left: nodeID_L = 1 - 1 (nodeID - 2)
path nodeIDs(TRUE=path, FALSE=WALL):
[0]wall top, [1]wall right, [2]wall bottom, [3]wall left,
explored(TRUE= unexplored, FALSE = unplored):
[4]top explored, [5]right explored, [6]bottom explored, [7]left explored
*/

//data-points for easy absolute clarification
#define WAL_T 0
#define WAL_R 1
#define WAL_B 2
#define WAL_L 3
#define EXP_T 4
#define EXP_R 5
#define EXP_B 6
#define EXP_L 7