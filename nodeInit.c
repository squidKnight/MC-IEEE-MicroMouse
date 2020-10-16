/*
Written by Mathazzar
Last modified: 10/13/20
Purpose:initialize the nodeList and mms sim.
Status: FINISHED, TESTED
*/

#include "API.h"
#include "mouseDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)

/*void nodeInit(short int nodeList[NODES][DATA])
INPUTS: bool nodeList[NODES][DATA]
	nodeList: the nodeList array
RETURNS: short int nodeList[NODES][DATA]
	nodeList: the nodeList array. Directly manipulated instead of being passed back.
NOTES:
	Primary function to interact with nodeList array for initialization.
	Assumes the micromouse is currently on the "bottom left" outside corner of the maze facing "up" and not facing outwards.
CAUTION:
	Manipulates the nodeList array directly and wipes all data from it.
*/
void nodeInit(bool nodeList[NODES][DATA]) //initialize nodeList
{
	simLog("Initializing nodeList...");

	//set all distances and backpaths to inifinity
	for (int i = 0; i<NODES; i++)
	{
		for (int j = 0; j < DATA; j++)
		{
			nodeList[i][j] = false;
		}
	}

	//initialize start node (which will be first in the stack)
	if (!API_wallLeft()) //if no wall, set next path on left to null ID
	{
		nodeList[0][WAL_L] = true;
		nodeList[0][EXP_L] = true;
	}
	if (!API_wallFront()) //if no wall, set next path in front to null ID
	{
		nodeList[0][WAL_T] = true;
		nodeList[0][EXP_T] = true;
	}
	if (!API_wallRight()) //if no wall, set next path on right to null ID
	{
		nodeList[0][WAL_R] = true;
		nodeList[0][EXP_R] = true;
	}

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
