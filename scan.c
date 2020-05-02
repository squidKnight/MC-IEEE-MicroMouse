/*
Written by squidKnight, Mathazzar
Last modified: 05/2/20
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
short int pathChooseAlt(int nodeList[NODES][DATA], int nodeCurrent, short int direction);
int stackBackpath(int nodeList[NODES][DATA], int nodeID, int nodePrevious, int distLastNode);

void scan(int nodeList[NODES][DATA])
{
	int position[2] = { 0, 0 };
	int distTotal = 0; //Current absolute distance from start
	short int distLastNode = 0;
	//short int dist = 0;
	//bool encounteredNode = false;
	short int direction = 0; //stores current orientation, 0 is starting direction (assumed to be upwards): 0 = up, 1 = right, 2 = down, 3 = left
	int nodePrevious = nodeList[0][0];
	int stack = 0;
	short int directionPrevious = direction; //stores direction that the last node exited from

	simLog("Begining maze scan...");
	while (1 == 1)
	{
		//continue till next node
		distLastNode += pathCheck(position, &direction);
		distTotal += distLastNode;
		simLog("\tEncountered node:");
		
		if (nodeCheck() == 1) //if maze node
		{
			simLog("\t\tNode class: Path node\n\t\tRecording node information...");
			API_setColor(position[0], position[1], 'B');
			int nodeID = getID(position);

			int rank = stackCheck(nodeList, nodeID);

			if (rank == INFINITY) //if node not on stack
			{
				//create node
				int nodeCurrent[DATA]; //stores all information on current node
				nodeCurrent[NODEID] = nodeID; //node ID
				nodeCurrent[DIST] = distTotal; //distance traveled
				nodeCurrent[NODEID_P] = nodePrevious; //current backpath
				setNodePath(updateDir(direction, 3), nodeCurrent, API_wallLeft()); //is left a wall?
				setNodePath(updateDir(direction, 0), nodeCurrent, API_wallFront()); //is front a wall?
				setNodePath(updateDir(direction, 1), nodeCurrent, API_wallRight()); //is right a wall?
				addNodePath(direction, nodeCurrent, nodeList[stack], nodePrevious, directionPrevious);

				//add node to stack
				fprintf(stderr, "%d, %d, %d, %d\n", nodeCurrent[NODEID_T], nodeCurrent[NODEID_R], nodeCurrent[NODEID_B], nodeCurrent[NODEID_L]);
				fflush(stderr);
				stack = stackInsert(nodeList, nodeCurrent);

				//choose path
				if (!API_wallFront())
				{
					direction = updateDir(direction, 0);
				}
				else if (!API_wallLeft())
				{
					API_turnLeft();
					direction = updateDir(direction, 3);
				}
				else if (!API_wallRight())
				{
					API_turnRight();
					direction = updateDir(direction, 1);
				}
				else
				{
					simLog("ERROR: expected node class to be 1, not -1 DEADEND");
				}
			}
			else //if node already on stack
			{
				addNodePath(direction, nodeList[rank], nodeList[stack], nodePrevious, directionPrevious); //add the new path
				stack = rank;

				if (nodePrevious == nodeID) //if a direct loopback occured
				{
					simLog("Direct loopback occured, treating as deadend.");
					//Which direction is the loopback?
					if (nodePrevious == nodeList[rank][NODEID_T])
					{
						switch (direction)
						{
						case 0:
							nodeList[rank][EXP_B] = 1;
							nodeList[rank][EXP_T] = 1;
							break;
						case 1:
							nodeList[rank][EXP_L] = 1;
							nodeList[rank][EXP_T] = 1;
							break;
						case 2:
							simLog("Actual deadend, so there...");
							break;
						case 3:
							nodeList[rank][EXP_R] = 1;
							nodeList[rank][EXP_T] = 1;
							break;
						}
					}
					else if (nodePrevious == nodeList[rank][NODEID_R])
					{
						switch (direction)
						{
						case 0:
							nodeList[rank][EXP_B] = 1;
							nodeList[rank][EXP_R] = 1;
							break;
						case 1:
							nodeList[rank][EXP_L] = 1;
							nodeList[rank][EXP_R] = 1;
							break;
						case 2:
							nodeList[rank][EXP_T] = 1;
							nodeList[rank][EXP_R] = 1;
							break;
						case 3:
							simLog("Actual deadend, so there...");
							break;
						}
					}
					else if (nodePrevious == nodeList[rank][NODEID_B])
					{
						switch (direction)
						{
						case 0:
							simLog("Actual deadend, so there...");
							break;
						case 1:
							nodeList[rank][EXP_L] = 1;
							nodeList[rank][EXP_B] = 1;
							break;
						case 2:
							nodeList[rank][EXP_T] = 1;
							nodeList[rank][EXP_B] = 1;
							break;
						case 3:
							nodeList[rank][EXP_R] = 1;
							nodeList[rank][EXP_B] = 1;
							break;
						}
					}
					else if (nodePrevious == nodeList[rank][NODEID_L])
					{
						switch (direction)
						{
						case 0:
							nodeList[rank][EXP_B] = 1;
							nodeList[rank][EXP_L] = 1;
							break;
						case 1:
							simLog("Actual deadend, so there...");
							break;
						case 2:
							nodeList[rank][EXP_T] = 1;
							nodeList[rank][EXP_L] = 1;
							break;
						case 3:
							nodeList[rank][EXP_R] = 1;
							nodeList[rank][EXP_L] = 1;
							break;
						}
					}
					else
						simLog("ERROR: expected loopback, but can't find directionPrevious.");
				}
				else //reasses if current backpath is still the shortest available route back to start
				{
					if ((nodeList[stack][DIST] + distLastNode) < nodeList[rank][DIST]) //if new path is a shorter route for nodeCurrent
					{
						simLog("Shorter path for current node discovered; recalculating current node's backpath and its childeren's backpaths...");
						rank = stackBackpath(nodeList, nodeID, nodePrevious, distLastNode);
						simLog("Resuming path selection...");
					}
					else if ((nodeList[rank][DIST] + distLastNode) < nodeList[stack][DIST]) //if new path is a shorter route for nodePrevious
					{
						simLog("Shorter path for previous node discovered; recalculating previous node's backpath and its childeren's backpaths...");
						rank = stackBackpath(nodeList, nodePrevious, nodeID, distLastNode);
						simLog("Resuming path selection...");
					}
				}

				direction = pathChooseAlt(nodeList, direction, rank);
			}
			
			//Move beyond current node
			switch (direction) //if node fully explored
			{
			case 0:
			{
				if ((nodeList[stack][EXP_R] == 1) && (nodeList[stack][EXP_B] == 1) && (nodeList[stack][EXP_L] == 1))
					nodeList[stack][EXP_T] = 1;
				break;
			}
			case 1:
			{
				if ((nodeList[stack][EXP_T] == 1) && (nodeList[stack][EXP_B] == 1) && (nodeList[stack][EXP_L] == 1))
					nodeList[stack][EXP_R] = 1;
				break;
			}
			case 2:
			{
				if ((nodeList[stack][EXP_T] == 1) && (nodeList[stack][EXP_R] == 1) && (nodeList[stack][EXP_L] == 1))
					nodeList[stack][EXP_B] = 1;
				break;
			}
			case 3:
			{
				if ((nodeList[stack][EXP_T] == 1) && (nodeList[stack][EXP_R] == 1) && (nodeList[stack][EXP_B] == 1))
					nodeList[stack][EXP_L] = 1;
				break;
			}
			}
			API_moveForward();
			updatePos(position, direction, 1);
			distLastNode = 1;

			nodePrevious = nodeID; //current node will be the next one's backpath
			directionPrevious = direction; //record the current node's exit direction
		}
		else if (nodeCheck() == -1) //if node is a deadend
		{
			simLog("\t\tNode class: deadend\n\t\tReturning to previous node...");
			API_setColor(position[0], position[1], 'R');

			//return to last node
			API_turnRight();
			API_turnRight();
			direction = updateDir(direction, 2);
			pathCheck(position, &direction);

			//set direction as fully explored; treat it as a wall
			switch (direction)
			{
			case 0: //facing up: came from bottom
				nodeList[stack][EXP_B] = 1;
				nodeList[stack][NODEID_B] = INFINITY;
				break;
			case 1: //facing right: came from left
				nodeList[stack][EXP_L] = 1;
				nodeList[stack][NODEID_L] = INFINITY;
				break;
			case 2: //facing down: came from top
				nodeList[stack][EXP_T] = 1;
				nodeList[stack][NODEID_T] = INFINITY;
				break;
			case 3: //facing left: came from right
				nodeList[stack][EXP_R] = 1;
				nodeList[stack][NODEID_R] = INFINITY;
				break;
			}
			distTotal = nodeList[stack][DIST];
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

static void setNodePath(short int direction, int nodeCurrent[DATA], bool wall)
{
	switch (direction)
	{
	case 0:
	{
		if (!wall)
		{
			nodeCurrent[NODEID_T] = 0;
			nodeCurrent[EXP_T] = 0;
		}
		else if (wall)
		{
			nodeCurrent[EXP_T] = 1;
		}
		break;
	}
	case 1:
	{
		if (!wall)
		{
			nodeCurrent[NODEID_R] = 0;
			nodeCurrent[EXP_R] = 0;
		}
		else if (wall)
		{
			nodeCurrent[EXP_R] = 1;
		}
		break;
	}
	case 2:
	{
		if (!wall)
		{
			nodeCurrent[NODEID_B] = 0;
			nodeCurrent[EXP_B] = 0;
		}
		else if (wall)
		{
			nodeCurrent[EXP_B] = 1;
		}
		break;
	}
	case 3:
	{
		if (!wall)
		{
			nodeCurrent[NODEID_L] = 0;
			nodeCurrent[EXP_L] = 0;
		}
		else if (wall)
		{
			nodeCurrent[EXP_L] = 0;
		}
		break;
	}
	}
}

//nodeCurrent: current node, nodeStack: previous node, nodePrevious: previous node's nodeID, directionPrevious: previous node's exit direction
static bool addNodePath(short int direction, int nodeCurrent[DATA], int nodeStack[DATA], int nodePrevious, short int directionPrevious)
{
	if (nodeStack[NODEID] != nodePrevious)
	{
		simLog("ERROR: nodeID missmatch.");
		return false;
	}
	switch (directionPrevious) //record this node as a route available to nodePrevious from its last exit direction
	{
	case 0:
		nodeStack[NODEID_T] = nodeCurrent[NODEID];
		break;
	case 1:
		nodeStack[NODEID_R] = nodeCurrent[NODEID];
		break;
	case 2:
		nodeStack[NODEID_B] = nodeCurrent[NODEID];
		break;
	case 3:
		nodeStack[NODEID_L] = nodeCurrent[NODEID];
		break;
	}
	switch (updateDir(direction, 2)) //set backpath
	{
	case 0:
	{
		nodeCurrent[NODEID_T] = nodePrevious;
		if ((nodeStack[EXP_T] == 1) && (nodeStack[EXP_R] == 1) && (nodeStack[EXP_B] == 1) && (nodeStack[EXP_L] == 1))
			nodeCurrent[EXP_T] = 1;
		else
			nodeCurrent[EXP_T] = 0;
		break;
	}
	case 1:
	{
		nodeCurrent[NODEID_R] = nodePrevious;
		if ((nodeStack[EXP_T] == 1) && (nodeStack[EXP_R] == 1) && (nodeStack[EXP_B] == 1) && (nodeStack[EXP_L] == 1))
			nodeCurrent[EXP_R] = 1;
		else
			nodeCurrent[EXP_R] = 0;
		break;
	}
	case 2:
	{
		nodeCurrent[NODEID_B] = nodePrevious;
		if ((nodeStack[EXP_T] == 1) && (nodeStack[EXP_R] == 1) && (nodeStack[EXP_B] == 1) && (nodeStack[EXP_L] == 1))
			nodeCurrent[EXP_B] = 1;
		else
			nodeCurrent[EXP_B] = 0;
		break;
	}
	case 3:
	{
		nodeCurrent[NODEID_L] = nodePrevious;
		if ((nodeStack[EXP_T] == 1) && (nodeStack[EXP_R] == 1) && (nodeStack[EXP_B] == 1) && (nodeStack[EXP_L] == 1))
			nodeCurrent[EXP_L] = 1;
		else
			nodeCurrent[EXP_L] = 0;
		break;
	}
	}
	return true;
}
