/*
Written by squidKnight, Mathazzar
Last modified: 04/26/20
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
bool stackRemove(int nodeList[NODES][DATA], int rank);

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
				addNodePath(direction, nodeCurrent, nodeList[stack], nodePrevious);

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
				//Move beyond current node
				API_moveForward();
				updatePos(position, direction, 1);
				distLastNode = 1;
			}
			else //if node already on stack
			{
				addNodePath(direction, nodeList[rank], nodeList[stack], nodePrevious); //add the new path
				//reasses if current backpath is still the shortest available route back to start
				/*if (nodeList[rank][DIST] > distTotal) //is this route shorter? If so; update backpath. //has same problem as longer route case, but needs to check the childeren instead
				{
					int nodeCurrent[DATA]; //stores all information on current node
					for (int i = 0; i < DATA; i++) //copy old node information to temporary array
					{
						nodeCurrent[i] = nodeList[rank][i];
					}
					nodeCurrent[DIST] = distTotal; //new distance traveled
					nodeCurrent[NODEID_P] = nodePrevious; //new backpath
					stack = stackInsert(nodeList, nodeCurrent);
					stackRemove(nodeList, rank); //remove the old entry from the stack
				}
				else if ((nodeList[rank][DIST] + distLastNode) < distTotal) //is this route longer? //Do we care?? (much harder to implement)
				{
					//set new backpath
					nodeList[stack][DIST] = nodeList[rank][DIST] + distLastNode; //new distance traveled
					nodeList[stack][NODEID_P] = nodeID; //new backpath

					//check all parents //would it be better to use a recursive function? Do we have the memory and processing power to handle recursion?
					int rank2 = rank;
					int stack2 = stack;
					int nodeID2 = nodeID;
					int diff = distLastNode;
					while ((nodeList[rank2][DIST]) < (nodeList[stack2][DIST]))
					{
						diff = nodeList[stack2][DIST] - nodeList[rank2][DIST];
						//set new backpath
						nodeList[stack2][DIST] = nodeList[rank2][DIST] + diff; //new distance traveled
						nodeList[stack2][NODEID_P] = nodeID2; //new backpath
					}

					//resort the nodeList array
				}*/

				direction = pathChooseAlt(nodeList, direction, rank);
				API_moveForward();
				updatePos(position, direction, 1);
				distLastNode = 1;
			}
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

			nodePrevious = nodeID; //current node will be the next one's backpath
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

//nodeCurrent: current node, nodeStack: previous node, nodePrevious: previous node's nodeID
static bool addNodePath(short int direction, int nodeCurrent[DATA], int nodeStack[DATA], int nodePrevious)
{
	if (nodeStack[NODEID] != nodePrevious)
	{
		simLog("ERROR: nodeID missmatch.");
		return false;
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
