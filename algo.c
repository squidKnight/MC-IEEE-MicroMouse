/*
Written by squidKnight
Last modified: 03/15/20
Purpose: hold all of the alorithm-related maze functions (scaning, solving, optimizing, etc.)
Status: UNFINISHED, NOT TESTED

NOTE: ONLY COMPATABLE IN SIMULATOR!! Need to translate and incorperate finished movement/sensor functions once algorithm in stable
NOTE: most of the simulator-based functions start with "API_" so any that have this prefix need to be replaced for the physical bot
*/

#define SIM_MODE //remove once outside of sim environment
#define INFINITY 1024 //highly unlikely to ever have this value for a distance, so can be "infinity"

//need to put these declarations in a separate header file...
void nodeInit(); //initialize nodeList
void scan(int (*nodeList)[256][3]); //will A* be incorperated into this step?
int nodeCheck(); //checks if current position is a node or not

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

void nodeInit() //initialize nodeList
{
	simLog("Initializing nodeList...");
	int nodeList[256][3] = {}; //first dimension is node ID (second dimension: 0 = distance traveled from last node, 1 = backpath (previous node), 2 = node type (explorable or not))
	simLog("\tCreated nodelist");
	
	//initialize start node
	nodeList[1][0] = 0; //set start point (node ID 1) to have a distance of 0
	nodeList[1][1] = 0; //set start point to have a backpath of 1 (to self)
	nodeList[1][2] = 0; //set start point to have a node type of 0 (non-explorable, not a real node)
	API_setColor(0,0,'Y'); //visual mark
	simLog("\tStarting position marked and recorded");
	
	//mark goal position (IDs of 120, 121, 136, 137)
	API_setColor(7,7,'Y');
	API_setColor(7,8,'Y');
	API_setColor(8,7,'Y');
	API_setColor(8,8,'Y');
	simLog("\tGoal marked");

	//set all distances and backpaths to inifinity
	int i, j;
	for(i=2; i<254; i++) //node ID 0 does not exist and ID 1 is already recorded as start
	{
		for(j=0; j<3; j++)
		{
			nodeList[i][j] = INFINITY;
		}
	}
	simLog("\tAll possible nodes inilialized");
	simLog("Initilization completed sucessfully!\n");
	scan(&nodeList); //using pointer instead of global definition of nodeList (allows for potential to multi-thread)
}

void scan(int (*nodeList)[256][3]) //will A* be incorperated into this step?
{
	simLog("Begining maze scan...");
	int dist = 0;
	int direction = 0; //holds changes in orentation
	while(nodeCheck() == 0) //NOTE: the simulator cannot detect walls that are not next to it (this implementation will need to change for physical bot)
	{
		API_moveForward();
		dist ++; //this needs to be replaced with motor functions to determie wall lengths traveled
	}
	simLog("\tEncountered first non-forward case");
}

int nodeCheck() //checks if current position is a node or not
{
	//count number of avalible paths
	int status, paths = 3;
	if(API_wallFront())
		paths--;
	if(API_wallRight())
		paths--;
	if(API_wallLeft())
		paths--;

	//determine status
	switch (paths)
	{
		case 0: //deadend
			status = -1;
			break;
		case 1: //not a normal node: is it a corner or a clear path?
			status = 0; //assume it will be a clear path
			if(API_wallFront()) //corner condition
				status = 2;
			break;
		case 2: //is a node, add to stack
			status = 1;
			break;
		default:
			status = -2; //should not happen...
	}
	return status;
}

void solve() //not sure if needed, may be able to do in scan() by nature of dijkstras
{
}

void optimize() //optimizes path by utilizing diagonals
{
}
