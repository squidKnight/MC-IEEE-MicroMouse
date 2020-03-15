/*
Written by squidKnight
Last modified: 03/15/20
Purpose: hold all of the alorithm-related maze functions (scaning, solving, optimizing, etc.)
Status: UNFINISHED, NOT TESTED

NOTE: only compatable in simulator!! Need to translate and incorperate finished movement/sensor functions once algorithm in stable
*/
#define SIM_MODE //remove once outside of sim environment
#define INFINITY 1024 //highly unlikely to ever have this value for a distance, so can be "infinity"

#ifdef SIM_MODE
	#include "API.h"
	#include <stdio.h>
	void simLog(char* text) //modified from main.c in mms example (https://github.com/mackorone/mms-c)
	{
		fprintf(stderr, "%s\n", text);
		fflush(stderr);
	}
	
	void translate() //translates cartesian system used in simulator to 16x16 matrix
	{
	}
#endif

void scan() //will A* be incorperated into this step?
{
	int nodeList[256][3] = {}; //first dimension is node ID (second dimension: 0 = distance, 1 = backpath, 2 = node type)
	simLog("created nodelist");
	
	nodeList[1][0] = 0; //set start point (node ID 1) to have a distance of 0
	nodeList[1][1] = 0; //set start point to have a backpath of 1 (to self)
	nodeList[1][2] = 0; //set start point to have a node type of 0 (non-explorable)
	simLog("starting position recorded");
}

void solve() //not sure if needed, may be able to do in scan() by nature of dijkstras
{
}

void optimize() //optimizes path by utilizing diagonals
{
}
