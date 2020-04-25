/*
Written by Mathazzar
Last modified: 04/43/20
Purpose: choose next non-corner/deadend node to take.
Status: UNFINISHED, PARTIALLY TESTED
*/

#include "API.h"
#include <stdio.h>
#include <stdbool.h>
#include "mouseDefs.h"

int backpath_classful(int position[2], short int direction);
int nodeCheck(); 
short int changeDir(short int direction, short int newDirection);

short int pathChooseAlt(int nodeCurrent[DATA], short int direction, int position[2])
{
	fprintf(stderr, "Change from %d to %d\n", direction, nodeCurrent[6]);
	fflush(stderr);
	short int dire = changeDir(direction, nodeCurrent[6]);

	fprintf(stderr, "%d, %d, %d\n", nodeCurrent[3], nodeCurrent[4], nodeCurrent[5]);
	fflush(stderr);

	if (nodeCurrent[4] >= INFINITY - 1) //If wall or deadened in front: try going right, then left
	{
		if ((nodeCurrent[3] > nodeCurrent[5]) && (nodeCurrent[3] < INFINITY-1))
		{
			simLog("Picking Left:0");
			API_turnLeft();
			dire = updateDir(dire, 3);
		}
		else
		{
			simLog("Picking Right:0");
			API_turnRight();
			dire = updateDir(dire, 1);
		}
	}
	else if (nodeCurrent[3] >= INFINITY - 1) //If wall or deadened to left: try going right, then straight
	{
		if ((nodeCurrent[4] > nodeCurrent[5]) && (nodeCurrent[4] < INFINITY - 1))
		{
			simLog("Picking Straight:1");
		}
		else
		{
			simLog("Picking Right:1");
			API_turnRight();
			dire = updateDir(dire, 1);
		}
	}
	else if (nodeCurrent[5] >= INFINITY - 1) //If wall or deadened to right: try going left, then straight
	{
		if ((nodeCurrent[4] > nodeCurrent[3]) && (nodeCurrent[4] < INFINITY - 1))
		{
			simLog("Picking Straight:2");
		}
		else
		{
			simLog("Picking Left:2");
			API_turnLeft();
			dire = updateDir(dire, 3);
		}
	}
	else //if 3 paths available, pick second-shortest/longest
	{
		if (((nodeCurrent[4] >= nodeCurrent[3]) && (nodeCurrent[4] <= nodeCurrent[5])) || ((nodeCurrent[4] >= nodeCurrent[5]) && (nodeCurrent[4] <= nodeCurrent[3]))) //Front path is 2nd shortest
		{
			simLog("Picking Straight:3");
		}
		else if (((nodeCurrent[3] >= nodeCurrent[4]) && (nodeCurrent[3] <= nodeCurrent[5])) || ((nodeCurrent[3] >= nodeCurrent[5]) && (nodeCurrent[3] <= nodeCurrent[4]))) //Left path is 2nd shortest
		{
			simLog("Picking Left:3");
			API_turnLeft();
			dire = updateDir(dire, 3);
		}
		else if (((nodeCurrent[5] >= nodeCurrent[3]) && (nodeCurrent[5] <= nodeCurrent[4])) || ((nodeCurrent[5] >= nodeCurrent[4]) && (nodeCurrent[5] <= nodeCurrent[3]))) //Right path is 2nd shortest
		{
			simLog("Picking Right:3");
			API_turnRight();
			dire = updateDir(dire, 1);
		}
	}

	API_moveForward();
	updatePos(position, dire, 1);
	return dire;
}
