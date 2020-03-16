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
int getID(int direction, int dist, int position[2]); //gets the ID of the current node

#ifdef SIM_MODE
	#include "API.h"
	#include <stdio.h>
	void simLog(char* text) //modified from main.c in mms example (https://github.com/mackorone/mms-c)
	{
		fprintf(stderr, "%s\n", text);
		fflush(stderr);
	}

	int translate(int x, int y) //translates values from cartesian system used in simulator to 16x16 matrix node IDs
	{
		int nodeID = (x + (y * 16) + 1); //conversion from cartesian to matrixi
		fprintf(stderr, "\t\tNode ID: %d\n", nodeID);
		fflush(stderr);
		return nodeID;
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
	API_setText(0, 0, "start");
	simLog("\tStarting position marked and recorded");

	//mark goal position (IDs of 120, 121, 136, 137)
	API_setColor(7,7,'Y');
	API_setColor(7,8,'Y');
	API_setColor(8,8,'Y');
	API_setColor(8,7,'Y');
	API_setText(7, 7, "Goal");
	API_setText(7, 8, "Goal");
	API_setText(8, 8, "Goal");
	API_setText(8, 7, "Goal");
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
	int position[2] = {0, 0}; //current x and y position
	int dist = 0; //0 = up, 1 = down, 2 = right, 3 = left
	int direction = 0; //holds changes in orentation
	
	simLog("Begining maze scan...");
	int type = nodeCheck(); //holds type of node
	while(type == 0)
	{
		API_moveForward();
		dist ++; //this needs to be replaced with motor functions to determie wall lengths traveled
		type = nodeCheck();
	}
	simLog("\tEncountered wall(s)");
	getID(direction, dist, position);
	
	//do what needs to be done, depending on case
	if(type == -1) //if deadend
	{
		simLog("\t\tNode type: Dead-end");
		API_setColor(position[0], position[1], 'R');
	}
	if(type == 1) //if maze node
	{
		simLog("\t\tNode type: Maze node");
		API_setColor(position[0], position[1], 'B');
	}
	if(type == 2) //if corner
	{
		simLog("\t\tNode type: Corner node");
		API_setColor(position[0], position[1], 'G');
	}
}

int getID(int direction, int dist, int position[2]) //gets the ID of the current node
{
	//determine change in x and y
	if(direction == 0) //if facing up
		position[1] += dist; //y position increased (normally, this would be incremented by traveled distance recorded by encoders)
	if(direction == 1) //if facing down
		position[1] -= dist;
	if(direction == 2) //if facing right
		position[0] += dist; //x position increased
	if(direction == 3) //if facing left
		position[0] -= dist;
	
	//translate to matrix format, this can be removed once outside of simulator
	return translate(position[0], position[1]);
}

int nodeCheck() //checks if current position is a node or not
{
	//count number of avalible paths
	int status, paths = 3;
	if(API_wallFront()) //NOTE: the simulator cannot detect walls that are not next to it (this implementation will need to change for physical bot)
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
