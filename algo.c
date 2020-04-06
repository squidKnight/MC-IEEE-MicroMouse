/*
Written by squidKnight, Mathazzar
Last modified: 03/31/20
Purpose: hold all of the alorithm-related maze functions (scaning, solving, optimizing, etc.)
Status: UNFINISHED, NOT TESTED

NOTE: will probably need to divide functions into separate C files
NOTE: ONLY COMPATABLE IN SIMULATOR!! Need to translate and incorperate finished movement/sensor functions once algorithm in stable
NOTE: most of the simulator-based functions start with "API_" so any that have this prefix need to be replaced for the physical bot
*/

#include <stdbool.h>	
#include <stdio.h>
#include "API.h" //only needed for simulator use

#define INFINITY 1024 //highly unlikely to ever have this value for a distance, so can be "infinity"

//need to put these declarations in a separate header file...
int nodeCheck(); //checks if current position is a node or not
int getID(int direction, int dist, int position[2]); //gets the ID of the current node
bool stackInsert(int nodeCurrent[4]); //inserts new node into correct rank in stack based on distance
void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)

//NOTE: if multithreading, remove from global scope and pass via pointers instead
int nodeList[256][4]; //first dimension is ranking in stack (second dimension: 0 = nodeID, 1= distance traveled from last node, 2 = backpath (previous node), 3 = node type (explorable or not))

void nodeInit() //initialize nodeList
{
	simLog("Initializing nodeList...");

	//set all distances and backpaths to inifinity
	int i, j;
	for(i=0; i<256; i++)
	{
		for(j=0; j<4; j++)
		{
			nodeList[i][j] = INFINITY;
		}
	}

	//initialize start node (which will be first in the stack)
	nodeList[0][0] = 1; //set node ID to 1
	nodeList[0][1] = 0; //set start point to have a distance of 0
	nodeList[0][2] = 1; //set start point to have a backpath of 1 (to self)
	nodeList[0][3] = 0; //set start point to have a node type of 0 (non-explorable, not a real node)
	API_setColor(0,0,'Y'); //visual mark
	API_setText(0, 0, "start");

	//mark goal position (IDs of 120, 121, 136, 137)
	API_setColor(7,7,'Y');
	API_setColor(7,8,'Y');
	API_setColor(8,8,'Y');
	API_setColor(8,7,'Y');
	API_setText(7, 7, "Goal");
	API_setText(7, 8, "Goal");
	API_setText(8, 8, "Goal");
	API_setText(8, 7, "Goal");

	simLog("Initilization completed\n- - - - -");
}

void scan() //will A* be incorperated into this step?
{
	int position[2] = {0, 0}; //current x and y position
	int dist = 0; //total distance, do NOT reset!
	int direction = 0; //stores current orentation, 0 is starting orientation (assumed to be upwards): 0 = up, 1 = down, 2 = right, 3 = left
	int nodePrevious = 1; //stores the ID of the previous node

	simLog("Begining maze scan...");
	int nodeClass = nodeCheck(); //temporarily stores a node's class: path node, corner, or deadend
	while(nodeClass == 0) //waits until there is a node detected
	{
		API_moveForward();
		dist ++; //this needs to be replaced with motor functions to determie wall lengths traveled
		nodeClass = nodeCheck();
	}
	simLog("\tEncountered node:");
	int nodeID = getID(direction, dist, position); //gets the ID at the current position

	//do what needs to be done, depending on case
	switch(nodeClass)
	{
	case -1: //if deadend
	{
		simLog("\t\tNode class: Dead-end\n\t\tReturning to previous node...");
		API_setColor(position[0], position[1], 'R');
		break;
	}
	case 1: //if maze node
	{
		simLog("\t\tNode class: Path node\n\t\tRecording node information...");
		API_setColor(position[0], position[1], 'B');

		int nodeCurrent[4]; //stores all information on current node
		nodeCurrent[0] = nodeID; //node ID
		nodeCurrent[1] = dist; //distance traveled
		nodeCurrent[2] = nodePrevious; //backpath
		nodeCurrent[3] = 1; //is an explorable node
		nodePrevious = nodeID; //current node will be the next one's backpath
		stackInsert(nodeCurrent); //inserts the node into the stack
		break;
	}
	case 2: //if corner
	{
		simLog("\t\tNode class: Corner\n\t\tRecording node information...");
		API_setColor(position[0], position[1], 'G');
		int nodeCurrent[4];
		nodeCurrent[0] = nodeID;
		nodeCurrent[1] = dist;
		nodeCurrent[2] = nodePrevious;
		nodeCurrent[3] = 0; //is NOT an explorable node
		nodePrevious = nodeID;
		stackInsert(nodeCurrent);
		break;
	}
	}
}

bool stackInsert(int nodeCurrent[4]) //adds new node into correct rank in stack based on distance
{
	simLog("\t\tInserting node to stack...");
	bool rankFound = 0; //becomes 1 when stack rank for new node is found
	int tempArr[4]; //temporary storage array to allow for swaping in stack
	for (int i = 1; i < 256; i++) //hope element 256 has nothing in it... (skipped in this loop)
	{
		if (nodeList[i - 1][0] != INFINITY) //if a recorded node
		{
			fprintf(stderr, "\t\t\tRank of node %d: %d \n", nodeList[i - 1][0], i);
			fflush(stderr);
		}

		if ((nodeList[i - 1][1] < nodeCurrent[1]) && (nodeList[i][1] > nodeCurrent[1])) //if node higher in stack is less in distance and node lower in stack is greater in distance  than current node (ie, distance of current node lies inbetween those two values)
			rankFound = 1;
		//if the current node is a blank node, all existing nodes have been shifted over and the loop can end
		//if the current node already exists on the stack, then it doesn't need to be added to the stack, and cthe loop can end
		if (nodeCurrent[0] == INFINITY)//if current node is blank
			return rankFound;
		else if (nodeList[i][0] == nodeCurrent[0])//if current node has same ID
		{
			simLog("Node already exists.");
			fflush(stderr);
			return false;
		}

		//starting inserting nodes down the stack
		if (rankFound)
		{
			//store information of node in ranking below current node to temporary array
			tempArr[0] = nodeList[i][0];
			tempArr[1] = nodeList[i][1];
			tempArr[2] = nodeList[i][2];
			tempArr[3] = nodeList[i][3];

			//add current node to the stack
			nodeList[i][0] = nodeCurrent[0];
			nodeList[i][1] = nodeCurrent[1];
			nodeList[i][2] = nodeCurrent[2];
			nodeList[i][3] = nodeCurrent[3];

			//treat the node in temporary storage as the new current node
			nodeCurrent[0] = tempArr[0];
			nodeCurrent[1] = tempArr[1];
			nodeCurrent[2] = tempArr[2];
			nodeCurrent[3] = tempArr[3];
		}
	}
	simLog("ERROR: NO SPACE LEFT ON STACK");
}


void solve() //not sure if needed, may be able to do in scan() by nature of dijkstras
{
}

void optimize() //optimizes path by utilizing diagonals
{
}
