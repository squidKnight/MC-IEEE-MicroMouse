/*
Written by squidKnight, Mathazzar
Last modified: 05/13/20
Purpose: scan the maze.
Status: UNFINISHED, TESTED
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
int stackInsert(int nodeList[NODES][DATA], int nodeCurrent[DATA]);
int stackCheck(int nodeList[NODES][DATA], int nodeCurrent); //adds new node into correct rank in stack based on distance
short int pathChooseAlt(int nodeList[NODES][DATA], int nodeCurrent, short int direction);
int stackBackpath(int nodeList[NODES][DATA], int nodeID, int nodePrevious, int distLastNode);
int pathCheck(int position[2], short int *dire);

static void setNodePath(short int direction, int nodeCurrent[DATA], bool wall);
static bool addNodePath(short int direction, int nodeCurrent[DATA], int nodeStack[DATA], int nodePrevious, short int directionPrevious);

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
				addNodePath(direction, nodeCurrent, nodeList[stack], nodePrevious, directionPrevious); //add backpath

				//add node to stack
				fprintf(stderr, "%d, %d, %d, %d, direction: %d\n", nodeCurrent[NODEID_T], nodeCurrent[NODEID_R], nodeCurrent[NODEID_B], nodeCurrent[NODEID_L], direction);
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

				if (nodePrevious == nodeID) //if a direct loopback occured
				{
					simLog("Direct loopback occured, treating as deadend.");
					//Which direction is the loopback?
					if (nodePrevious == nodeList[rank][NODEID_T])
					{
						switch (directionPrevious)
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
							nodeList[rank][EXP_T] = 1;
							break;
						case 3:
							nodeList[rank][EXP_R] = 1;
							nodeList[rank][EXP_T] = 1;
							break;
						}
					}
					else if (nodePrevious == nodeList[rank][NODEID_R])
					{
						switch (directionPrevious)
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
							nodeList[rank][EXP_R] = 1;
							break;
						}
					}
					else if (nodePrevious == nodeList[rank][NODEID_B])
					{
						switch (directionPrevious)
						{
						case 0:
							simLog("Actual deadend, so there...");
							nodeList[rank][EXP_B] = 1;
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
						switch (directionPrevious)
						{
						case 0:
							nodeList[rank][EXP_B] = 1;
							nodeList[rank][EXP_L] = 1;
							break;
						case 1:
							simLog("Actual deadend, so there...");
							nodeList[rank][EXP_L] = 1;
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
					fprintf(stderr, "NODEID: %d, DIST: %d, NODEID_P: %d, NODEID_T: %d, NODEID_R: %d, NODEID_B: %d, NODEID_L: %d, EXP_T: %d, EXP_R: %d, EXP_B: %d, EXP_L: %d\n", nodeList[rank][NODEID], nodeList[rank][DIST], nodeList[rank][NODEID_P], nodeList[rank][NODEID_T], nodeList[rank][NODEID_R], nodeList[rank][NODEID_B], nodeList[rank][NODEID_L], nodeList[rank][EXP_T], nodeList[rank][EXP_R], nodeList[rank][EXP_B], nodeList[rank][EXP_L]);
					fflush(stderr);
					if ((nodeList[stack][DIST] + distLastNode) < nodeList[rank][DIST]) //if new path is a shorter route for nodeCurrent
					{
						simLog("Shorter path for current node discovered; recalculating current node's backpath and its childeren's backpaths...");
						int rankTest = stackBackpath(nodeList, nodeID, nodePrevious, distLastNode);
						if (rankTest != rank)
						{
							fprintf(stderr, "ERROR: recursive function thinks nodeCurrent is %d, when it should be %d.", rankTest, rank);
							fflush(stderr);
						}
						simLog("Resuming path selection...");
					}
					else if ((nodeList[rank][DIST] + distLastNode) < nodeList[stack][DIST]) //if new path is a shorter route for nodePrevious
					{
						fprintf(stderr, "NODEID: %d, DIST: %d, NODEID_P: %d, NODEID_T: %d, NODEID_R: %d, NODEID_B: %d, NODEID_L: %d, EXP_T: %d, EXP_R: %d, EXP_B: %d, EXP_L: %d\n", nodeList[stack][NODEID], nodeList[stack][DIST], nodeList[stack][NODEID_P], nodeList[stack][NODEID_T], nodeList[stack][NODEID_R], nodeList[stack][NODEID_B], nodeList[stack][NODEID_L], nodeList[stack][EXP_T], nodeList[stack][EXP_R], nodeList[stack][EXP_B], nodeList[stack][EXP_L]);
						fflush(stderr);
						simLog("Shorter path for previous node discovered; recalculating previous node's backpath and its childeren's backpaths...");
						int rankTest = stackBackpath(nodeList, nodePrevious, nodeID, distLastNode);
						if (rankTest != stack)
						{
							fprintf(stderr, "ERROR: recursive function thinks nodePrevious is %d, when it should be %d.", rankTest, stack);
							fflush(stderr);
						}
						simLog("Resuming path selection...");
					}
				}

				fprintf(stderr, "NODEID: %d, DIST: %d, NODEID_P: %d, NODEID_T: %d, NODEID_R: %d, NODEID_B: %d, NODEID_L: %d, EXP_T: %d, EXP_R: %d, EXP_B: %d, EXP_L: %d\n", nodeList[rank][NODEID], nodeList[rank][DIST], nodeList[rank][NODEID_P], nodeList[rank][NODEID_T], nodeList[rank][NODEID_R], nodeList[rank][NODEID_B], nodeList[rank][NODEID_L], nodeList[rank][EXP_T], nodeList[rank][EXP_R], nodeList[rank][EXP_B], nodeList[rank][EXP_L]);
				fflush(stderr);

				direction = pathChooseAlt(nodeList, rank, direction);
				stack = rank;
			}
			
			//Move beyond current node
			switch (direction) //if node fully explored
			{
			case 0:
			{
				if ((nodeList[stack][EXP_R] == 1) && (nodeList[stack][EXP_B] == 1) && (nodeList[stack][EXP_L] == 1))
				{
					nodeList[stack][EXP_T] = 1;
					API_setColor(position[0], position[1], 'Y');
					fprintf(stderr, "node %d fully explored, exiting up.\nNODEID: %d, DIST: %d, NODEID_P: %d, NODEID_T: %d, NODEID_R: %d, NODEID_B: %d, NODEID_L: %d, EXP_T: %d, EXP_R: %d, EXP_B: %d, EXP_L: %d\n", nodeList[stack][NODEID], nodeList[rank][NODEID], nodeList[rank][DIST], nodeList[rank][NODEID_P], nodeList[rank][NODEID_T], nodeList[rank][NODEID_R], nodeList[rank][NODEID_B], nodeList[rank][NODEID_L], nodeList[rank][EXP_T], nodeList[rank][EXP_R], nodeList[rank][EXP_B], nodeList[rank][EXP_L]);
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
					fprintf(stderr, "node %d fully explored, exiting right.\nNODEID: %d, DIST: %d, NODEID_P: %d, NODEID_T: %d, NODEID_R: %d, NODEID_B: %d, NODEID_L: %d, EXP_T: %d, EXP_R: %d, EXP_B: %d, EXP_L: %d\n", nodeList[stack][NODEID], nodeList[rank][NODEID], nodeList[rank][DIST], nodeList[rank][NODEID_P], nodeList[rank][NODEID_T], nodeList[rank][NODEID_R], nodeList[rank][NODEID_B], nodeList[rank][NODEID_L], nodeList[rank][EXP_T], nodeList[rank][EXP_R], nodeList[rank][EXP_B], nodeList[rank][EXP_L]);
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
					fprintf(stderr, "node %d fully explored, exiting down.\nNODEID: %d, DIST: %d, NODEID_P: %d, NODEID_T: %d, NODEID_R: %d, NODEID_B: %d, NODEID_L: %d, EXP_T: %d, EXP_R: %d, EXP_B: %d, EXP_L: %d\n", nodeList[stack][NODEID], nodeList[rank][NODEID], nodeList[rank][DIST], nodeList[rank][NODEID_P], nodeList[rank][NODEID_T], nodeList[rank][NODEID_R], nodeList[rank][NODEID_B], nodeList[rank][NODEID_L], nodeList[rank][EXP_T], nodeList[rank][EXP_R], nodeList[rank][EXP_B], nodeList[rank][EXP_L]);
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
					fprintf(stderr, "node %d fully explored, exiting left.\nNODEID: %d, DIST: %d, NODEID_P: %d, NODEID_T: %d, NODEID_R: %d, NODEID_B: %d, NODEID_L: %d, EXP_T: %d, EXP_R: %d, EXP_B: %d, EXP_L: %d\n", nodeList[stack][NODEID], nodeList[rank][NODEID], nodeList[rank][DIST], nodeList[rank][NODEID_P], nodeList[rank][NODEID_T], nodeList[rank][NODEID_R], nodeList[rank][NODEID_B], nodeList[rank][NODEID_L], nodeList[rank][EXP_T], nodeList[rank][EXP_R], nodeList[rank][EXP_B], nodeList[rank][EXP_L]);
					fflush(stderr);
				}
				break;
			}
			}
			if (!API_wallFront()) //verify that this is a valid move to prevent position errors
			{
				API_moveForward();
				updatePos(position, direction, 1);
				distLastNode = 1;
			}
			else
			{
				simLog("FATAL ERROR: expected valid direction to be picked, but there's a wall in front.");
				break;
			}

			nodePrevious = nodeID; //current node will be the next one's backpath
			directionPrevious = direction; //record the current node's exit direction
		}
		else if (nodeCheck() == -1) //if node is a deadend
		{
			simLog("\t\tNode class: deadend\n\t\tReturning to previous node...");
			API_setColor(position[0], position[1], 'R');

			//return to last node, it will treat the deadend as a loopback
			API_turnRight();
			API_turnRight();
			direction = updateDir(direction, 2);
			nodePrevious = getID(position); //set as nodePrevious to force error on addNodePath() because stack doesn't match
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
			simLog("FATAL X position ERROR");
			break;
		}
		if (position[1] < 0 || position[1] >= 16)
		{
			simLog("FATAL Y position ERROR");
			break;
		}
	}
	for (int i = 0; i < NODES; i++)
	{
		fprintf(stderr, "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", i, nodeList[i][NODEID], nodeList[i][DIST], nodeList[i][NODEID_P], nodeList[i][NODEID_T], nodeList[i][NODEID_R], nodeList[i][NODEID_B], nodeList[i][NODEID_L], nodeList[i][EXP_T], nodeList[i][EXP_R], nodeList[i][EXP_B], nodeList[i][EXP_L]);
		fflush(stderr);
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
			//simLog("wall above");
			nodeCurrent[NODEID_T] = INFINITY;
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
			//simLog("wall right");
			nodeCurrent[NODEID_R] = INFINITY;
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
			//simLog("wall below");
			nodeCurrent[NODEID_B] = INFINITY;
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
			//simLog("wall left");
			nodeCurrent[NODEID_L] = INFINITY;
			nodeCurrent[EXP_L] = 1;
		}
		break;
	}
	}
}

//nodeCurrent: current node, nodeStack: previous node, nodePrevious: previous node's nodeID, directionPrevious: previous node's exit direction
static bool addNodePath(short int direction, int nodeCurrent[DATA], int nodeStack[DATA], int nodePrevious, short int directionPrevious)
{
	fprintf(stderr, "adding nodePrevious (%d) as path connected to nodeCurrent (%d).\n", nodePrevious, nodeCurrent[NODEID]);
	fflush(stderr);
	if (nodeStack[NODEID] != nodePrevious) //verify nodePrevious is the same node as nodeStack passed to function. If nodePrevious was a deadend, it should result in this, preventing incorrect edits.
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
	switch (direction) //add new path to nodeCurrent
	{
	case 0: //facing up; it's down
	{
		nodeCurrent[NODEID_B] = nodePrevious;
		if ((nodeStack[EXP_T] == 1) && (nodeStack[EXP_R] == 1) && (nodeStack[EXP_B] == 1) && (nodeStack[EXP_L] == 1))
			nodeCurrent[EXP_B] = 1;
		else
			nodeCurrent[EXP_B] = 0;
		break;
	}
	case 1: //facing right; it's left
	{
		nodeCurrent[NODEID_L] = nodePrevious;
		if ((nodeStack[EXP_T] == 1) && (nodeStack[EXP_R] == 1) && (nodeStack[EXP_B] == 1) && (nodeStack[EXP_L] == 1))
			nodeCurrent[EXP_L] = 1;
		else
			nodeCurrent[EXP_L] = 0;
		break;
	}
	case 2: //facing down; it's up
	{
		nodeCurrent[NODEID_T] = nodePrevious;
		if ((nodeStack[EXP_T] == 1) && (nodeStack[EXP_R] == 1) && (nodeStack[EXP_B] == 1) && (nodeStack[EXP_L] == 1))
			nodeCurrent[EXP_T] = 1;
		else
			nodeCurrent[EXP_T] = 0;
		break;
	}
	case 3: //facing left; it's right
	{
		nodeCurrent[NODEID_R] = nodePrevious;
		if ((nodeStack[EXP_T] == 1) && (nodeStack[EXP_R] == 1) && (nodeStack[EXP_B] == 1) && (nodeStack[EXP_L] == 1))
			nodeCurrent[EXP_R] = 1;
		else
			nodeCurrent[EXP_R] = 0;
		break;
	}
	}
	return true;
}
