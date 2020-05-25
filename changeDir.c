/*
Written by Mathazzar
Last modified: 04/24/20
Purpose: change the dirction the micromouse to a specific desired direction.
Status: FINISHED, TESTED
*/

#include "API.h"
#include <stdbool.h>
#include <stdio.h>


void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)
short int updateDir(short int direction, short int relativeChange);

/*short int changeDir(short int direction, short int newDirection)
INPUTS: short int direction, short int newDirection
	direction: current orientation of micromouse relative to original orientation
	newDirection: new orientation of micromouse relative to original orientation
RETURNS: short int dire
	dire: new varuable that gives the new direction the micromouse is facing
NOTES:
	implemented to be called as:
		direction = changeDir(direction, newDirection);
		to update direction to new direction
	0=up/straight, 1=right, 2=down/about face, 3=left
*/
short int changeDir(short int direction, short int newDirection)
{
	short int dire = direction;
	simLog("\tChanging direction...");
	
	switch (direction)
	{
	case 0:
		switch (newDirection)
		{
		case 0:
			simLog("Going Straight:0");
			break;
		case 1:
			simLog("Turning Left:0");
			API_turnLeft();
			dire = updateDir(dire, 3);
			break;
		case 2:
			simLog("About Face:0");
			API_turnRight();
			API_turnRight();
			dire = updateDir(dire, 2);
			break;
		case 3:
			simLog("Turning Right:0");
			API_turnRight();
			dire = updateDir(dire, 1);
			break;
		default:
			simLog("DIRECTION ERROR:0");
			break;
		}
		break;
	case 1:
		switch (newDirection)
		{
		case 0:
			simLog("Turning Left:1");
			API_turnRight();
			dire = updateDir(dire, 1);
			break;
		case 1:
			simLog("Going Straight:1");
			break;
		case 2:
			simLog("Turning Left:1");
			API_turnLeft();
			dire = updateDir(dire, 3);
			break;
		case 3:
			simLog("About Face:1");
			API_turnRight();
			API_turnRight();
			dire = updateDir(dire, 2);
			break;
		default:
			simLog("DIRECTION ERROR:1");
			break;
		}
		break;
	case 2:
		switch (newDirection)
		{
		case 0:
			simLog("About Face:2");
			API_turnRight();
			API_turnRight();
			dire = updateDir(dire, 2);
			break;
		case 1:
			simLog("Turning Left:2");
			API_turnLeft();
			dire = updateDir(dire, 3);
			break;
		case 2:
			simLog("Going Straight:2");
			break;
		case 3:
			simLog("Turning Right:2");
			API_turnRight();
			dire = updateDir(dire, 1);
			break;
		default:
			simLog("DIRECTION ERROR:2");
			break;
		}
		break;
	case 3:
		switch (newDirection)
		{
		case 0:
			simLog("Turning Left:3");
			API_turnLeft();
			dire = updateDir(dire, 3);
			break;
		case 1:
			simLog("About Face:3");
			API_turnRight();
			API_turnRight();
			dire = updateDir(dire, 2);
			break;
		case 2:
			simLog("Turning Right:3");
			API_turnRight();
			dire = updateDir(dire, 1);
			break;
		case 3:
			break;
		default:
			simLog("DIRECTION ERROR:3");
			break;
		}
		break;
	}

	if (dire != newDirection)
		simLog("DIRECTION ERROR");
	return dire;
}
