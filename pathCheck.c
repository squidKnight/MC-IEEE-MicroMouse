/*
Written by Mathazzar
Last modified: 10/06/20
Purpose: move to next node.
Status: FINISHED, TESTED
*/

#include "API.h"
#include <stdio.h>
#include <stdbool.h>
#include "mouseDefs.h"

short int nodeCheck(); //checks to see if the current location is a node
short int updateDir(short int direction, short int relativeChange); //updates the direction the micromouse is facing
void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)
void updatePos(short int position[2], short int direction, short int dist); //updates the position of the micromouse

/*int checkPath(int position[2], short int *dire)
INPUTS: int position[2], short int *dire
	position: the current position of the micromouse. Implemented for use with the position array in scan.c
	dire:  the current direction of the micromouse in relation to its original orientation. Needs a pointer to directly manipulate external data.
RETURNS: int distLastNode, short int *dire
	distLastNode: How far the micromouse has traveled since it was last at a node.
	dire: the new direction of the micromouse. Directly manipulated instead of being passed back.
NOTES:
	Implemented to be called as:
		distLastNode = checkPath(position, &dire);
CAUTION:
	Manipulates the direction variable passed to it directly using pointers, requires the & prefix to update the direction and remember the new orientation externally.
*/
short int pathCheck(short int position[2], short int *dire)
{
	short int dist = 0;
	short int distLastNode = 0;

	while (nodeCheck() == 0)
	{
		if (!API_wallFront())
		{

		}
		else if (!API_wallRight())
		{
			updatePos(position, *dire, dist);
			API_setColor(position[0], position[1], 'G');
			API_turnRight();
			distLastNode += dist + 1;
			*dire = updateDir(*dire, 1);
			dist = 0;
		}
		else if (!API_wallLeft())
		{
			updatePos(position, *dire, dist);
			API_setColor(position[0], position[1], 'G');
			API_turnLeft();
			distLastNode += dist + 1;
			*dire = updateDir(*dire, 3);
			dist = 0;
		}
		else
		{
			simLog("ERROR: expected node class to be 0, not -1 DEADEND");
		}
		API_moveForward();
		dist++;
	}
	updatePos(position, *dire, dist);
	distLastNode += dist;
	return distLastNode;
}
