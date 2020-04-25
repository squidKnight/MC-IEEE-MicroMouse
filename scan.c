/*
Written by squidKnight, Mathazzar
Last modified: 04/24/20
Purpose: scan the maze.
*/

#include <stdbool.h>
#include <stdio.h>
#include "API.h" //only needed for simulator use
#include "mouseDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)
int nodeCheck();
void updatePos(int position[2], short int direction, short int dist);
short int updateDir(short int direction, short int relativeChange);
int getID(int position[2]);
short int pathChoose(int nodeCurrent[DATA], int paths, short int direction, int position[2]);
int stackInsert(int nodeList[NODES][DATA], int nodeCurrent[DATA]);
int backpath_classful(int position[2], short int direction);
int stackCheck(int nodeList[NODES][DATA], int nodeCurrent); //adds new node into correct rank in stack based on distance
short int pathChooseAlt(int nodeCurrent[DATA], short int direction, int position[2]);

void scan(int nodeList[256][DATA][3])
{
	int position[2] = { 0, 0 };
	int distTotal = 0;
	//short int distLastNode = 0;
	short int dist = 0;
	//bool encounteredNode = false;
	short int direction = 0; //stores current orientation, 0 is starting direction (assumed to be upwards): 0 = up, 1 = right, 2 = down, 3 = left
	int nodePrevious = nodeList[0][0];
	int stack = 0;

	simLog("Begining maze scan...");
	while (1 == 1)
	{
		//reset distances traveled since last node
		short int dist = 0;

		//continue till next node
		distTotal = pathCheck(position, &direction);
		simLog("\tEncountered node:");
		//updatePos(position, direction, dist);

		//distLastNode += dist;
		distTotal += dist;
		int nodeID = getID(position);

		if (nodeCheck() == 1) //if maze node
		{
			simLog("\t\tNode class: Path node\n\t\tRecording node information...");
			API_setColor(position[0], position[1], 'B');

			//create node
			int paths = 0;
			int nodeCurrent[DATA]; //stores all information on current node
			nodeCurrent[0] = nodeID; //node ID
			nodeCurrent[1] = distTotal; //distance traveled
			nodeCurrent[2] = nodePrevious; //backpath
			if (!API_wallLeft())
			{
				nodeCurrent[3] = -1; //if no wall, set distance to next path on left to 0
				paths++;
			}
			else
				nodeCurrent[3] = INFINITY - 1;
			if (!API_wallFront())
			{
				nodeCurrent[4] = -1; //if no wall, set distance to next path to front to 0
				paths++;
			}
			else
				nodeCurrent[4] = INFINITY - 1;
			if (!API_wallRight())
			{
				nodeCurrent[5] = -1; //if no wall, set distance to next path on right to 0
				paths++;
			}
			else
				nodeCurrent[5] = INFINITY - 1;
			nodeCurrent[6] = direction;
			nodeCurrent[7] = paths + nodeList[stack][7];

			//choose path
			fprintf(stderr, "%d, %d, %d\n", nodeCurrent[3], nodeCurrent[4], nodeCurrent[5]);
			fflush(stderr);
			int rank = stackCheck(nodeList, nodeCurrent[0]);
			if (rank == INFINITY) //if node already on stack
			{
				direction = pathChoose(nodeCurrent, paths, direction, position);
				//add node to stack
				stack = stackInsert(nodeList, nodeCurrent);
			}
			else
			{
				direction = pathChooseAlt(nodeList[rank], direction, position);
				stack = rank;
			}

			nodePrevious = nodeID; //current node will be the next one's backpath
		}
		else
		{
			simLog("NODE ERROR");
		}

		//vvvv debug code vvvv
		if (position[0] < 0 || position[0] >= 16)
		{
			simLog("X position ERROR");
			break;
		}
		if (position[1] < 0 || position[1] >= 16)
		{
			simLog("Y position ERROR");
			break;
		}
	}
}
