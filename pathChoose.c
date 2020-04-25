/*
Written by Mathazzar
Last modified: 04/24/20
Purpose: choose next non-corner node to take.
Status: FINISHED, TESTED
*/

#include "API.h"
#include <stdio.h>
#include <stdbool.h>
#include "mouseDefs.h"

int backpath_classful(int position[2], short int direction);
int nodeCheck();
short int updateDir(short int direction, short int relativeChange);
void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)
void updatePos(int position[2], short int direction, short int dist);
int pathCheck(int position[2], short int *dire);

/*short int choosePath(int nodeCurrent[DATA], int paths, short int direction, int position[2])
INPUTS: int nodeCurrent[DATA], int paths, short int direction, int position[2]
	nodeCurrent[DATA]: the current node the micromouse is at and must choose the next path for
	paths: the number of possible paths the micromouse can explore at nodeCurrent[DATA]
	direction: the current direction of the micromouse in relation to its original orientation.
	position[2]: the current position of the micromouse. Implemented for use with the position array in scan.c
RETURNS: dire
	dire: a new variable for the new direction, which is returned.
NOTES:
	implemented to be called as:
		direction = choosePath(int nodeCurrent[DATA], int paths, short int direction, int position[2]);
		to set direction to new orientation once it has returned.
CAUTION:
	Manipulates the position array passed to it directly via updatePos.
	Manipulates the nodeCurrent array passed to it directly.
*/
short int pathChoose(int nodeCurrent[DATA], int paths, short int direction, int position[2])
{
	//Initialize variables for internal use
	short int dire = direction;
	int path = paths;
	//bool deadend = false;

	//Check each path
	//Left Path
	if (nodeCurrent[3] == -1)
		{
			simLog("Checking left path...");
			API_turnLeft();
			dire = updateDir(dire, 3);

			int distLastNode = pathCheck(position, &dire);
			
			if (nodeCheck() == 1)
				nodeCurrent[3] = distLastNode;
			else if (nodeCheck() == -1)
			{
				API_setColor(position[0], position[1], 'R');
				nodeCurrent[3] = INFINITY - 1;
				nodeCurrent[7]--;
			}
			//same for each check
			path--;
			/*fprintf(stderr, "distance: %d, paths left: %d\n", distLastNode, paths);
			fflush(stderr);*/

			//reset
			if (path > 0)
			{
				dire = backpath_classful(position, dire);
				API_turnLeft();
				dire = updateDir(dire, 3);
			}
			else if (path == 0)
			{
				simLog("Picking Left");
				return dire;
			}
			else
				simLog("PATHS ERROR 0");
		}
	//Front Path
	if (nodeCurrent[4] == -1)
	{
		simLog("Checking front path...");

		int distLastNode = pathCheck(position, &dire);

		if (nodeCheck() == 1)
			nodeCurrent[4] = distLastNode;
		else if (nodeCheck() == -1)
		{
			API_setColor(position[0], position[1], 'R');
			nodeCurrent[4] = INFINITY - 1;
			nodeCurrent[7]--;
		}
		//same for each check
		path--;
		/*fprintf(stderr, "distance: %d, paths left: %d\n", distLastNode, paths);
		fflush(stderr);*/

		//reset
		if (path > 0)
		{
			dire = backpath_classful(position, dire);
			API_turnRight();
			API_turnRight();
			dire = updateDir(dire, 2);
		}
		else if (path == 0)
		{
			if (nodeCurrent[3] < nodeCurrent[4])
			{
				dire = backpath_classful(position, dire);
				API_turnRight();
				dire = updateDir(dire, 1);
				simLog("Picking Left");
				API_moveForward();
				updatePos(position, dire, 1);
				return dire;
			}
			else
			{
				simLog("Picking Front");
				return dire;
			}
		}
		else
		{
			simLog("PATHS ERROR 1");
		}
	}
	//Right Path
	if (nodeCurrent[5] == -1)
	{
		simLog("Checking right path...");
		API_turnRight();
		dire = updateDir(dire, 1);

		int distLastNode = pathCheck(position, &dire);

		if (nodeCheck() == 1)
			nodeCurrent[5] = distLastNode;
		else if (nodeCheck() == -1)
		{
			API_setColor(position[0], position[1], 'R');
			nodeCurrent[5] = INFINITY - 1;
			nodeCurrent[7]--;
		}
		//same for each check
		path--;
		/*fprintf(stderr, "distance: %d, paths left: %d\n", distLastNode, paths);
		fflush(stderr);*/

		if (path > 0)
			simLog("PATHS ERROR 2");
		//Choose path
		if ((nodeCurrent[3] <= nodeCurrent[4]) && (nodeCurrent[3] < nodeCurrent[5]))
		{
			dire = backpath_classful(position, dire);
			simLog("Picking Left");
			API_moveForward();
			updatePos(position, dire, 1);
			return dire;
		}
		else if (nodeCurrent[4] < nodeCurrent[5])
		{
			dire = backpath_classful(position, dire);
			API_turnRight();
			dire = updateDir(dire, 1);
			simLog("Picking Front");
			API_moveForward();
			updatePos(position, dire, 1);
			return dire;
		}
		else
		{
			simLog("Picking Right");
			return dire;
		}
	}
	//Should never be reached
	simLog("PATHING ERROR");
}