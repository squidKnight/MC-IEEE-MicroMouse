/*
Written by Mathazzar
Last modified: 10/06/20
Purpose: update the direction the micromouse is facing
Status: FINISHED, TESTED
*/

#include <stdbool.h>
#include <stdio.h>

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)

/*short int updateDir(short int direction, short int relativeChange)
INPUTS: short int direction, short int relativeChange
	direction: current orientation of micromouse relative to original orientation
	relativeChange: change in orientation of micromouse relative to current orientation
RETURNS: short int dire
	dire: new varuable that gives the new direction the micromouse is facing
NOTES:
	implemented to be called as:
		direction = updateDir(direction, relativeChange);
		to update direction to new direction
	0=up/straight, 1=right, 2=down/about face, 3=left
*/
short int updateDir(short int direction, short int relativeChange)
{
	short int dire = direction;
	switch (relativeChange)
	{
	case 0: //straight
		break;
	case 1: //turn right
		dire++;
		if (dire > 3)
			dire -= 4;
		break;
	case 2: //about face
		dire += 2;
		if (dire > 3)
			dire -= 4;
		break;
	case 3: //turn left
		dire--;
		if (dire < 0)
			dire += 4;
		break;
	default:
		simLog("DIRECTION UPDATE ERROR");
		break;
	}
	return dire;
}