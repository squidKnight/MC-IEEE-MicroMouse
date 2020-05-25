/*
Written by Mathazzar
Last modified: 05/24/20
Purpose: choose next not fully explored node and travel to it.
Status: FINISHED, TESTED
*/

#include "API.h"
#include <stdio.h>
#include <stdbool.h>
#include "mouseDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)
int stackCheck(int nodeList[NODES][DATA], int nodeCurrent); //adds new node into correct rank in stack based on distance
short int updateDir(short int direction, short int relativeChange);
int getID(int position[2]);
int pathCheck(int position[2], short int *dire);
void updatePos(int position[2], short int direction, short int dist);

/*short int pathChooseAlt(int nodeList[NODES][DATA], int nodeCurrent,short int direction, int position[2])
INPUTS: int nodeList[NODES][DATA], int nodeCurrent, short int direction, int position[2]
	nodeList: the nodeList array.
	nodeCurrent: rank of the current node on the stack that the micromouse is at and must choose the next path for.
	direction: the current direction of the micromouse in relation to its original orientation.
	position: the current position of the micromouse. Implemented for use with the position array in scan.c
RETURNS: short int dire, int nodeList[NODES][DATA]
	dire: a new variable for the new direction, which is returned.
	position: updates the position directly.
	nodeList: updates the explored directions directly.
NOTES:
	implemented to be called as:
		direction = pathChooseAlt(nodeList, nodeCurrent, direction, position);
		to set direction to new orientation once it has returned.
	Doesn't check for direct loopbacks, they need to be checked externally before this function executes.
	Manipulates nodeCurrent directly, though it's immutable and won't be affected when this fuction exits.
	Checks along backpaths for next route, so there will be cases where there's a closer node it can go to or
		a shorter path it can take to get there that it won't look for. An independent algorithm would be needed
		to check for that, which isn't currently implemented due to concerns with the Arduino's capabilities.
	Designed to intentionally crash and return to a higher function when no unexplored paths exist.
CAUTION:
	Manipulates the nodeList array passed to it directly.
	Manipulates the position array passed to it directly via updatePos().
*/
short int pathChooseAlt(int nodeList[NODES][DATA], int nodeCurrent, short int direction, int position[2])
{
	short int dire = direction;
	
	simLog("Searching for unexplored path along backpath...");
	int nodeID = getID(position);
	//verify that there's an explorable direction along backpath
	short int nodeCheck = nodeCurrent;
	bool found = false;
	while (nodeID != 1)
	{
		if ((nodeList[nodeCheck][EXP_T] == 0 && nodeList[nodeCheck][NODEID_T] == 0) || (nodeList[nodeCheck][EXP_R] == 0 && nodeList[nodeCheck][NODEID_R] == 0) || (nodeList[nodeCheck][EXP_B] == 0 && nodeList[nodeCheck][NODEID_B] == 0) || (nodeList[nodeCheck][EXP_L] == 0 && nodeList[nodeCheck][NODEID_L] == 0))
		{
			found = true;
			break;
		}

		if (nodeList[nodeCheck][NODEID_P] == nodeList[nodeCheck][NODEID_T])
		{
			nodeID = nodeList[nodeCheck][NODEID_T];
		}
		else if (nodeList[nodeCheck][NODEID_P] == nodeList[nodeCheck][NODEID_R])
		{
			nodeID = nodeList[nodeCheck][NODEID_R];
		}
		else if (nodeList[nodeCheck][NODEID_P] == nodeList[nodeCheck][NODEID_B])
		{
			nodeID = nodeList[nodeCheck][NODEID_B];
		}
		else if (nodeList[nodeCheck][NODEID_P] == nodeList[nodeCheck][NODEID_L])
		{
			nodeID = nodeList[nodeCheck][NODEID_L];
		}

		nodeCheck = stackCheck(nodeList, nodeID);
	
	}

	nodeID = getID(position);
	if (found)
	{
		while ((nodeID != 1) && !((nodeList[nodeCurrent][EXP_T] == 0 && nodeList[nodeCurrent][NODEID_T] == 0) || (nodeList[nodeCurrent][EXP_R] == 0 && nodeList[nodeCurrent][NODEID_R] == 0) || (nodeList[nodeCurrent][EXP_B] == 0 && nodeList[nodeCurrent][NODEID_B] == 0) || (nodeList[nodeCurrent][EXP_L] == 0 && nodeList[nodeCurrent][NODEID_L] == 0)))
		{
			short int front, right, back, left;
			switch (dire) //set directions for current orientation
			{
			case 0:
				front = NODEID_T;
				right = NODEID_R;
				back = NODEID_B;
				left = NODEID_L;
				break;
			case 1:
				front = NODEID_R;
				right = NODEID_B;
				back = NODEID_L;
				left = NODEID_T;
				break;
			case 2:
				front = NODEID_B;
				right = NODEID_L;
				back = NODEID_T;
				left = NODEID_R;
				break;
			case 3:
				front = NODEID_L;
				right = NODEID_T;
				back = NODEID_R;
				left = NODEID_B;
				break;
			}
			if (nodeList[nodeCurrent][NODEID_P] == nodeList[nodeCurrent][front])
			{
				dire = updateDir(dire, 0);
			}
			else if (nodeList[nodeCurrent][NODEID_P] == nodeList[nodeCurrent][right])
			{
				API_turnRight();
				dire = updateDir(dire, 1);
			}
			else if (nodeList[nodeCurrent][NODEID_P] == nodeList[nodeCurrent][back])
			{
				API_turnRight();
				API_turnRight();
				dire = updateDir(dire, 2);
			}
			else if (nodeList[nodeCurrent][NODEID_P] == nodeList[nodeCurrent][left])
			{
				API_turnLeft();
				dire = updateDir(dire, 3);
			}
			else
			{
				simLog("CRITICAL ERROR: expected backpath not among available routes.");
				break;
			}

			if (!API_wallFront())
			{
				//check if node is simulating a dead-end
				switch (direction) //if node fully explored
				{
				case 0:
				{
					if ((nodeList[nodeCurrent][EXP_R] == 1) && (nodeList[nodeCurrent][EXP_B] == 1) && (nodeList[nodeCurrent][EXP_L] == 1))
					{
						nodeList[nodeCurrent][EXP_T] = 1;
						API_setColor(position[0], position[1], 'Y');
						fprintf(stderr, "node %d fully explored, exiting up.\n", nodeList[nodeCurrent][NODEID]);
						fflush(stderr);
					}
					break;
				}
				case 1:
				{
					if ((nodeList[nodeCurrent][EXP_T] == 1) && (nodeList[nodeCurrent][EXP_B] == 1) && (nodeList[nodeCurrent][EXP_L] == 1))
					{
						nodeList[nodeCurrent][EXP_R] = 1;
						API_setColor(position[0], position[1], 'Y');
						fprintf(stderr, "node %d fully explored, exiting right.\n", nodeList[nodeCurrent][NODEID]);
						fflush(stderr);
					}
					break;
				}
				case 2:
				{
					if ((nodeList[nodeCurrent][EXP_T] == 1) && (nodeList[nodeCurrent][EXP_R] == 1) && (nodeList[nodeCurrent][EXP_L] == 1))
					{
						nodeList[nodeCurrent][EXP_B] = 1;
						API_setColor(position[0], position[1], 'Y');
						fprintf(stderr, "node %d fully explored, exiting down.\n", nodeList[nodeCurrent][NODEID]);
						fflush(stderr);
					}
					break;
				}
				case 3:
				{
					if ((nodeList[nodeCurrent][EXP_T] == 1) && (nodeList[nodeCurrent][EXP_R] == 1) && (nodeList[nodeCurrent][EXP_B] == 1))
					{
						nodeList[nodeCurrent][EXP_L] = 1;
						API_setColor(position[0], position[1], 'Y');
						fprintf(stderr, "node %d fully explored, exiting right.\n", nodeList[nodeCurrent][NODEID]);
						fflush(stderr);
					}
					break;
				}
				}

				API_moveForward();
				updatePos(position, dire, 1);
				pathCheck(position, &dire);

				nodeID = getID(position);
				nodeCurrent = stackCheck(nodeList, nodeID);
			}
			else
			{
				simLog("CRITICAL ERROR: didn't expect a wall.");
				break;
			}
		}
	}
	else
	{
		//find next node not fully explored
		int nodeNextID = nodeID;
		for (int i = 0; i < NODES; i++)
		{
			if ((nodeList[i][EXP_T] == 0 && nodeList[i][NODEID_T] == 0) || (nodeList[i][EXP_R] == 0 && nodeList[i][NODEID_R] == 0) || (nodeList[i][EXP_B] == 0 && nodeList[i][NODEID_B] == 0) || (nodeList[i][EXP_L] == 0 && nodeList[i][NODEID_L] == 0))
			{
				nodeNextID = nodeList[i][NODEID];
				fprintf(stderr, "NodeID %d has an unexplored path, calculating route...\n", nodeNextID);
				fflush(stderr);
				break;
			}
		}

		//calculate route from nodeNext to start
		simLog("calculating route from nodeNext to start...");
		int nodeNextPath[NODES / 8];
		nodeNextPath[0] = nodeNextID;
		for (int i = 1; i < NODES / 8; i++) //add backpath to nodeNextPath
		{
			nodeNextPath[i] = nodeList[stackCheck(nodeList, nodeNextPath[i - 1])][NODEID_P];
			if (nodeNextPath[i] == 1)
				break;
		}
		for (int i = 0; i < NODES / 8; i++)
		{
			fprintf(stderr, "%d; NodeID: %d\n", i, nodeNextPath[i]);
			fflush(stderr);
		}
		fflush(stderr);
		
		//traverse until a shared backpath is discovered
		short int x = 0;
		found = false;
		while (!found && nodeID != 1)
		{
			for (int i = 0; i < NODES / 8; i++)
			{
				if (nodeNextPath[i] == nodeID)
				{
					found = true;
					x = i - 1;
					break;
				}
			}
			if (found)
			{
				simLog("taking shortcut...");
			}
			else
			{
				short int front, right, back, left;
				switch (dire) //set directions for current orientation
				{
				case 0:
					front = NODEID_T;
					right = NODEID_R;
					back = NODEID_B;
					left = NODEID_L;
					break;
				case 1:
					front = NODEID_R;
					right = NODEID_B;
					back = NODEID_L;
					left = NODEID_T;
					break;
				case 2:
					front = NODEID_B;
					right = NODEID_L;
					back = NODEID_T;
					left = NODEID_R;
					break;
				case 3:
					front = NODEID_L;
					right = NODEID_T;
					back = NODEID_R;
					left = NODEID_B;
					break;
				}
				if (nodeList[nodeCurrent][NODEID_P] == nodeList[nodeCurrent][front])
				{
					dire = updateDir(dire, 0);
				}
				else if (nodeList[nodeCurrent][NODEID_P] == nodeList[nodeCurrent][right])
				{
					API_turnRight();
					dire = updateDir(dire, 1);
				}
				else if (nodeList[nodeCurrent][NODEID_P] == nodeList[nodeCurrent][back])
				{
					API_turnRight();
					API_turnRight();
					dire = updateDir(dire, 2);
				}
				else if (nodeList[nodeCurrent][NODEID_P] == nodeList[nodeCurrent][left])
				{
					API_turnLeft();
					dire = updateDir(dire, 3);
				}
				else
				{
					simLog("CRITICAL ERROR: expected backpath not among available routes.");
					break;
				}

				if (!API_wallFront())
				{
					//check if node is simulating a dead-end
					switch (direction) //if node fully explored
					{
					case 0:
					{
						if ((nodeList[nodeCurrent][EXP_R] == 1) && (nodeList[nodeCurrent][EXP_B] == 1) && (nodeList[nodeCurrent][EXP_L] == 1))
						{
							nodeList[nodeCurrent][EXP_T] = 1;
							API_setColor(position[0], position[1], 'Y');
							fprintf(stderr, "node %d fully explored, exiting up.\n", nodeList[nodeCurrent][NODEID]);
							fflush(stderr);
						}
						break;
					}
					case 1:
					{
						if ((nodeList[nodeCurrent][EXP_T] == 1) && (nodeList[nodeCurrent][EXP_B] == 1) && (nodeList[nodeCurrent][EXP_L] == 1))
						{
							nodeList[nodeCurrent][EXP_R] = 1;
							API_setColor(position[0], position[1], 'Y');
							fprintf(stderr, "node %d fully explored, exiting right.\n", nodeList[nodeCurrent][NODEID]);
							fflush(stderr);
						}
						break;
					}
					case 2:
					{
						if ((nodeList[nodeCurrent][EXP_T] == 1) && (nodeList[nodeCurrent][EXP_R] == 1) && (nodeList[nodeCurrent][EXP_L] == 1))
						{
							nodeList[nodeCurrent][EXP_B] = 1;
							API_setColor(position[0], position[1], 'Y');
							fprintf(stderr, "node %d fully explored, exiting down.\n", nodeList[nodeCurrent][NODEID]);
							fflush(stderr);
						}
						break;
					}
					case 3:
					{
						if ((nodeList[nodeCurrent][EXP_T] == 1) && (nodeList[nodeCurrent][EXP_R] == 1) && (nodeList[nodeCurrent][EXP_B] == 1))
						{
							nodeList[nodeCurrent][EXP_L] = 1;
							API_setColor(position[0], position[1], 'Y');
							fprintf(stderr, "node %d fully explored, exiting right.\n", nodeList[nodeCurrent][NODEID]);
							fflush(stderr);
						}
						break;
					}
					}

					API_moveForward();
					updatePos(position, dire, 1);
					pathCheck(position, &dire);

					nodeID = getID(position);
					nodeCurrent = stackCheck(nodeList, nodeID);
				}
				else
				{
					simLog("CRITICAL ERROR: didn't expect a wall.");
					break;
				}
			}
			
		}

		//traverse maze along chosen path to nodeNext
		while (nodeID != nodeNextID)
		{
			short int heap = stackCheck(nodeList, nodeID);
			if (x < 0)
			{
				fprintf(stderr, "CRITICAL ERROR: stack underflow: %d.", x);
				fflush(stderr);
				break;
			}
			else if (nodeID != nodeNextPath[x + 1])
			{
				simLog("CRITICAL ERROR: expected route not taken.");
				break;
			}
			short int front, right, back, left;
			switch (dire) //set directions for current orientation
			{
			case 0:
				front = NODEID_T;
				right = NODEID_R;
				back = NODEID_B;
				left = NODEID_L;
				break;
			case 1:
				front = NODEID_R;
				right = NODEID_B;
				back = NODEID_L;
				left = NODEID_T;
				break;
			case 2:
				front = NODEID_B;
				right = NODEID_L;
				back = NODEID_T;
				left = NODEID_R;
				break;
			case 3:
				front = NODEID_L;
				right = NODEID_T;
				back = NODEID_R;
				left = NODEID_B;
				break;
			}
			if (nodeNextPath[x] == nodeList[heap][front])
			{
				dire = updateDir(dire, 0);
			}
			else if (nodeNextPath[x] == nodeList[heap][right])
			{
				API_turnRight();
				dire = updateDir(dire, 1);
			}
			else if (nodeNextPath[x] == nodeList[heap][back])
			{
				API_turnRight();
				API_turnRight();
				dire = updateDir(dire, 2);
			}
			else if (nodeNextPath[x] == nodeList[heap][left])
			{
				API_turnLeft();
				dire = updateDir(dire, 3);
			}
			else
			{
				simLog("CRITICAL ERROR: expected backpath not among available routes.");
				break;
			}

			if (!API_wallFront())
			{
				//check if node is simulating a dead-end
				short int stack = stackCheck(nodeList, nodeID);
				switch (direction) //if node fully explored
				{
				case 0:
				{
					if ((nodeList[stack][EXP_R] == 1) && (nodeList[stack][EXP_B] == 1) && (nodeList[stack][EXP_L] == 1))
					{
						nodeList[stack][EXP_T] = 1;
						API_setColor(position[0], position[1], 'Y');
						fprintf(stderr, "node %d fully explored, exiting up.\n", nodeList[stack][NODEID]);
						fflush(stderr);
					}
					break;
				}
				case 1:
				{
					if ((nodeList[stack][EXP_T] == 1) && (nodeList[stack][EXP_B] == 1) && (nodeList[stack][EXP_L] == 1))
					{
						nodeList[stack][EXP_R] = 1;
						API_setColor(position[0], position[1], 'Y');
						fprintf(stderr, "node %d fully explored, exiting right.\n", nodeList[stack][NODEID]);
						fflush(stderr);
					}
					break;
				}
				case 2:
				{
					if ((nodeList[stack][EXP_T] == 1) && (nodeList[stack][EXP_R] == 1) && (nodeList[stack][EXP_L] == 1))
					{
						nodeList[stack][EXP_B] = 1;
						API_setColor(position[0], position[1], 'Y');
						fprintf(stderr, "node %d fully explored, exiting down.\n", nodeList[stack][NODEID]);
						fflush(stderr);
					}
					break;
				}
				case 3:
				{
					if ((nodeList[stack][EXP_T] == 1) && (nodeList[stack][EXP_R] == 1) && (nodeList[stack][EXP_B] == 1))
					{
						nodeList[stack][EXP_L] = 1;
						API_setColor(position[0], position[1], 'Y');
						fprintf(stderr, "node %d fully explored, exiting right.\n", nodeList[stack][NODEID]);
						fflush(stderr);
					}
					break;
				}
				}

				API_moveForward();
				updatePos(position, dire, 1);
				pathCheck(position, &dire);

				x--;
				nodeID = getID(position);
			}
			else
			{
				simLog("CRITICAL ERROR: didn't expect a wall.");
				break;
			}
		}
	}

	return dire;
}