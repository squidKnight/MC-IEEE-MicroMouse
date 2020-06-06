/*
Written by Mathazzar
Last modified: 05/13/20
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
	//fprintf(stderr, "DIRECTION: %d\n", dire);
	//fflush(stderr);
	switch (relativeChange)
	{
	case 0: //straight
		//simLog("Turning Straight?");
		break;
	case 1: //turn right
		//simLog("Turning Right");
		dire++;
		if (dire > 3)
			dire -= 4;
		//fprintf(stderr, "DIRECTION: %d\n", dire);
		//fflush(stderr);
		break;
	case 2: //about face
		//simLog("About Face");
		dire += 2;
		if (dire > 3)
			dire -= 4;
		//fprintf(stderr, "DIRECTION: %d\n", dire);
		//fflush(stderr);
		break;
	case 3: //turn left
		//simLog("Turning Left");
		dire--;
		if (dire < 0)
			dire += 4;
		//fprintf(stderr, "DIRECTION: %d\n", dire);
		//fflush(stderr);
		break;
	default:
		simLog("DIRECTION UPDATE ERROR");
		break;
	}
	return dire;
}
