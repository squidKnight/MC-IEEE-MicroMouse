/*
Written by Mathazzar
Last modified: 04/22/20
Purpose: return to previous non-corner node.
Status: UNFINISHED, PARTIALLY TESTED
*/

#include <stdbool.h>
#include <stdio.h>
#include "API.h"

int updateDir(short int direction, short int relativeChange);
void updatePos(int position[2], short int direction, short int dist);
int nodeCheck();
void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)

/*int backpath_classless()
{
	simLog("Returning to last node...");

	//about face
	API_turnRight();
	API_turnRight();
	API_moveForward();

	//backtrace to last node visited
	short int nodeClass = nodeCheck(); //temporarily stores a node's class: path node, corner, or deadend
	while (nodeCheck() != 1) //waits until there is a node detected
	{
		if (!API_wallFront())
		{
			
		}
		else if (!API_wallRight())
		{
			API_turnRight();
		}
		else if (!API_wallLeft())
		{
			API_turnLeft();
		}
		API_moveForward();
	}
	return 0;
}*/

/*int backpath(int position[2], short int direction)
INPUTS: position[2], direction
	position[2]: the current position of the micromouse. Implemented for use with the position array in scan.c
	direction: the current direction of the micromouse in relation to its original orientation.
RETURNS: dire
	dire: a new variable for the new direction, which is returned.
NOTES:
	implemented to be called as:
		direction = backpath(int startNode, short int direction);
		to set direction to new orientation once it has returned.
	Assumes the micromouse is facing the opposite direction it needs to backtrace to.
CAUTION:
	Manipulates the position array passed to it directly via updatePos.
*/
int backpath_classful(int position[2], short int direction)
{
	simLog("Returning to last node...");

	//Initialize variables for internal use
	short int dire = direction;
	int dist = 0;

	//about face
	API_turnRight();
	API_turnRight();
	dire = updateDir(dire, 2);
	API_moveForward();
	dist++;

	/*fprintf(stderr, "direction: %d\n", dire);
	fflush(stderr);*/

	//backtrace to last node visited
	while (nodeCheck() != 1) //waits until there is a node detected
	{
		if (!API_wallFront())
		{
			
		}
		else if (!API_wallRight())
		{
			updatePos(position, dire, dist);
			API_turnRight();
			dire = updateDir(dire, 1);
			dist = 0;
		}
		else if (!API_wallLeft())
		{
			updatePos(position, dire, dist);
			API_turnLeft();
			dire = updateDir(dire, 3);
			dist = 0;
		}
		else
		{
			simLog("WALL ERROR");
		}
		API_moveForward();
		dist++;
	}
	updatePos(position, dire, dist);

	simLog("Resuming...");
	return dire;
}