/*
Written by Mathazzar
Last modified: 06/05/20
Purpose:initialize the nodeList and mms sim.
Status: FINISHED, TESTED
*/

#include "API.h"
#include "mouseDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)

/*void nodeInit(short int nodeList[NODES][DATA])
INPUTS: short int nodeList[NODES][DATA]
	nodeList: the nodeList array
RETURNS: short int nodeList[NODES][DATA]
	nodeList: the nodeList array. Directly manipulated instead of being passed back.
NOTES:
	Primary function to interact with nodeList array for initialization.
	Assumes the micromouse is currently on an outside corner of the maze and not facing outwards.
CAUTION:
	Manipulates the nodeList array directly and wipes all data from it.
*/
void nodeInit(short int nodeList[NODES][DATA]) //initialize nodeList
{
	simLog("Initializing nodeList...");

	//set all distances and backpaths to inifinity
	for (int i = 0; i<NODES; i++)
	{
		for (int j = 0; j < DATA; j++)
		{
			nodeList[i][j] = INFINITY;
		}
	}

	//initialize start node (which will be first in the stack)
	nodeList[0][NODEID] = 1; //set node ID to 1
	if (!API_wallLeft())
		nodeList[0][NODEID_L] = 0; //if no wall, set next path on left to null ID
	if (!API_wallFront())
		nodeList[0][NODEID_T] = 0; //if no wall, set next path in front to null ID
	if (!API_wallRight())
		nodeList[0][NODEID_R] = 0; //if no wall, set next path on right to null ID

	API_setColor(0, 0, 'Y'); //visual mark
	API_setText(0, 0, "start");

	//mark goal position (IDs of 120, 121, 136, 137)
	API_setColor(7, 7, 'Y');
	API_setColor(7, 8, 'Y');
	API_setColor(8, 8, 'Y');
	API_setColor(8, 7, 'Y');
	API_setText(7, 7, "Goal");
	API_setText(7, 8, "Goal");
	API_setText(8, 8, "Goal");
	API_setText(8, 7, "Goal");

	simLog("Initilization completed\n- - - - -");
}
