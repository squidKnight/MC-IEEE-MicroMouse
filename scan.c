/*
Written by squidKnight, Mathazzar
Last modified: 05/29/20
Purpose: scan the maze.
Status: NOT FINISHED, TESTED
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
short int pathChoose(int nodeList[NODES][DATA], int nodeCurrent, short int direction);
int stackBackpath(int nodeList[NODES][DATA], int nodeID, int nodePrevious, int distLastNode);
int pathCheck(int position[2], short int *dire);
short int pathChooseAlt(int nodeList[NODES][DATA], int nodeCurrent, short int direction, int position[2]);

static void setNodePath(short int direction, int nodeCurrent[DATA], bool wall);
static bool addNodePath(short int direction, int nodeCurrent[DATA], int nodeStack[DATA], int nodePrevious, short int directionPrevious, int dist);

/*void scan(int nodeList[NODES][DATA])
INPUTS: int nodeList[NODES][DATA]
	nodeList: the nodeList array.
RETURNS: int nodeList[NODES][DATA]
	nodeList: updates the nodeList array directly.
NOTES:
	implemented to be called from main.c
	Designed to intentionally crash and return to a higher function when no unexplored paths exist.
	Master function for exploring the maze.
CAUTION:
	Manipulates the nodeList array directly.
	Internal [positioning variables are not stored universaly and will not be passed to the function that called it, they will have to be reconstructed from the nodeList array's data.
*/
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
	if (API_wallFront())
	{
		if (!API_wallLeft())
			directionPrevious = updateDir(direction, 3);
		else if (!API_wallRight())
			directionPrevious = updateDir(direction, 1);
		else
			directionPrevious = updateDir(direction, 2);
	}

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
				addNodePath(direction, nodeCurrent, nodeList[stack], nodePrevious, directionPrevious, distLastNode); //add backpath

				//add node to stack
				fprintf(stderr, "%d, %d, %d, %d, direction: %d\n\t%d, %d, %d, %d\n", nodeCurrent[NODEID_T], nodeCurrent[NODEID_R], nodeCurrent[NODEID_B], nodeCurrent[NODEID_L], direction, nodeCurrent[DIST_T], nodeCurrent[DIST_R], nodeCurrent[DIST_B], nodeCurrent[DIST_L]);
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
			else if (nodeID != nodeList[0][NODEID]) //if node already on stack
			{
				addNodePath(direction, nodeList[rank], nodeList[stack], nodePrevious, directionPrevious, distLastNode); //add the new path

				if (nodePrevious == nodeID) //if a direct loopback occured
				{
					simLog("Direct loopback occured, treating as deadend.");
					fprintf(stderr, "nodeID: %d, T: %d, R: %d, B: %d, L: %d\n", nodeList[rank][NODEID], nodeList[rank][NODEID_T], nodeList[rank][NODEID_R], nodeList[rank][NODEID_B], nodeList[rank][NODEID_L]);
					fflush(stderr);
					//Which direction is the loopback?
					if (nodePrevious == nodeList[rank][NODEID_T])
					{
						switch (direction)
						{
						case 0:
							simLog("T0");
							nodeList[rank][NODEID_B] = INFINITY;
							nodeList[rank][NODEID_T] = INFINITY;
							break;
						case 1:
							simLog("T1");
							nodeList[rank][NODEID_L] = INFINITY;
							nodeList[rank][NODEID_T] = INFINITY;
							break;
						case 2:
							simLog("Actual deadend, so there...T2");
							nodeList[rank][NODEID_T] = INFINITY;
							break;
						case 3:
							simLog("T3");
							nodeList[rank][NODEID_R] = INFINITY;
							nodeList[rank][NODEID_T] = INFINITY;
							break;
						}
					}
					else if (nodePrevious == nodeList[rank][NODEID_R])
					{
						switch (direction)
						{
						case 0:
							simLog("R0");
							nodeList[rank][NODEID_B] = INFINITY;
							nodeList[rank][NODEID_R] = INFINITY;
							break;
						case 1:
							simLog("R1");
							nodeList[rank][NODEID_L] = INFINITY;
							nodeList[rank][NODEID_R] = INFINITY;
							break;
						case 2:
							simLog("R2");
							nodeList[rank][NODEID_T] = INFINITY;
							nodeList[rank][NODEID_R] = INFINITY;
							break;
						case 3:
							simLog("Actual deadend, so there...R3");
							nodeList[rank][NODEID_R] = INFINITY;
							break;
						}
					}
					else if (nodePrevious == nodeList[rank][NODEID_B])
					{
						switch (direction)
						{
						case 0:
							simLog("Actual deadend, so there...B0");
							nodeList[rank][NODEID_B] = INFINITY;
							break;
						case 1:
							simLog("B1");
							nodeList[rank][NODEID_L] = INFINITY;
							nodeList[rank][NODEID_B] = INFINITY;
							break;
						case 2:
							simLog("B2");
							nodeList[rank][NODEID_T] = INFINITY;
							nodeList[rank][NODEID_B] = INFINITY;
							break;
						case 3:
							simLog("B3");
							nodeList[rank][NODEID_R] = INFINITY;
							nodeList[rank][NODEID_B] = INFINITY;
							break;
						}
					}
					else if (nodePrevious == nodeList[rank][NODEID_L])
					{
						switch (direction)
						{
						case 0:
							simLog("L0");
							nodeList[rank][NODEID_B] = INFINITY;
							nodeList[rank][NODEID_L] = INFINITY;
							break;
						case 1:
							simLog("Actual deadend, so there...L1");
							nodeList[rank][NODEID_L] = INFINITY;
							break;
						case 2:
							simLog("L2");
							nodeList[rank][NODEID_T] = INFINITY;
							nodeList[rank][NODEID_L] = INFINITY;
							break;
						case 3:
							simLog("L3");
							nodeList[rank][NODEID_R] = INFINITY;
							nodeList[rank][NODEID_L] = INFINITY;
							break;
						}
					}
					else
						simLog("ERROR: expected loopback, but can't find directionPrevious.");
					fprintf(stderr, "nodeID: %d, T: %d, R: %d, B: %d, L: %d\n", nodeList[rank][NODEID], nodeList[rank][NODEID_T], nodeList[rank][NODEID_R], nodeList[rank][NODEID_B], nodeList[rank][NODEID_L]);
					fflush(stderr);
				}
				else //reasses if current backpath is still the shortest available route back to start
				{
					fprintf(stderr, "NODEID: %d, DIST: %d, NODEID_P: %d, NODEID_T: %d, NODEID_R: %d, NODEID_B: %d, NODEID_L: %d\n", nodeList[rank][NODEID], nodeList[rank][DIST], nodeList[rank][NODEID_P], nodeList[rank][NODEID_T], nodeList[rank][NODEID_R], nodeList[rank][NODEID_B], nodeList[rank][NODEID_L]);
					fflush(stderr);
					if ((nodeList[stack][DIST] + distLastNode) < nodeList[rank][DIST]) //if new path is a shorter route for nodeCurrent
					{
						simLog("Shorter path for current node discovered; recalculating current node's backpath and its childeren's backpaths...");

						for (int i = 0; i < NODES; i++)
						{
							fprintf(stderr, "%d; nodeID: %d, backpath: %d\n", i, nodeList[i][NODEID], nodeList[i][NODEID_P]);
							fflush(stderr);
							if (nodeList[i][NODEID] == INFINITY)
								break;
						}

						int rankTest = stackBackpath(nodeList, nodeID, nodePrevious, distLastNode);

						for (int i = 0; i < NODES; i++)
						{
							fprintf(stderr, "%d; nodeID: %d, backpath: %d\n", i, nodeList[i][NODEID], nodeList[i][NODEID_P]);
							fflush(stderr);
							if (nodeList[i][NODEID] == INFINITY)
								break;
						}

						if (rankTest != rank)
						{
							fprintf(stderr, "ERROR: recursive function thinks nodeCurrent is %d, when it should be %d.", rankTest, rank);
							fflush(stderr);
						}
						simLog("Resuming path selection...");
					}
					else if ((nodeList[rank][DIST] + distLastNode) < nodeList[stack][DIST]) //if new path is a shorter route for nodePrevious
					{
						fprintf(stderr, "NODEID: %d, DIST: %d, NODEID_P: %d, NODEID_T: %d, NODEID_R: %d, NODEID_B: %d, NODEID_L: %d\n", nodeList[stack][NODEID], nodeList[stack][DIST], nodeList[stack][NODEID_P], nodeList[stack][NODEID_T], nodeList[stack][NODEID_R], nodeList[stack][NODEID_B], nodeList[stack][NODEID_L]);
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

				fprintf(stderr, "NODEID: %d, DIST: %d, NODEID_P: %d, NODEID_T: %d, NODEID_R: %d, NODEID_B: %d, NODEID_L: %d\n", nodeList[rank][NODEID], nodeList[rank][DIST], nodeList[rank][NODEID_P], nodeList[rank][NODEID_T], nodeList[rank][NODEID_R], nodeList[rank][NODEID_B], nodeList[rank][NODEID_L]);
				fflush(stderr);
				//set directions for current orientation
				short int front, right, back, left;
				switch (direction)
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
				fprintf(stderr, "nodeCurrent: %d, nodeID: %d,\t\tfront: %d, right: %d, back: %d, left: %d\n", rank, nodeList[rank][NODEID], nodeList[rank][front], nodeList[rank][right], nodeList[rank][back], nodeList[rank][left]);
				fflush(stderr);

				//Choose next available route, not previously traveled if possible
				if ((nodeList[rank][front] != 0) && (nodeList[rank][left] != 0) && (nodeList[rank][right] != 0)) //if no unexplored directions
				{
					simLog("Current node has no unexplored paths, searching for nearest node with unexplored paths...");
					
					short int san = 0;
					for (int i = 0; i < NODES; i++)
					{
						if ((nodeList[i][NODEID_T] == 0) || (nodeList[i][NODEID_R] == 0) || (nodeList[i][NODEID_B] == 0) || (nodeList[i][NODEID_L] == 0))
						{
							break;
						}
						san = i;
					}
					if (san == NODES - 1)
					{
						simLog("FATAL ERROR: nodeList array is either full or all nodes have been fully explored.");
						return;
					}
					else
					{
						fprintf(stderr, "%d\n", san);
						fflush(stderr);
					}
					
					direction = pathChooseAlt(nodeList, rank, direction, position);
					nodeID = getID(position);
					rank = stackCheck(nodeList, nodeID);
					distLastNode = nodeList[rank][DIST];
				}
				direction = pathChoose(nodeList, rank, direction); //pick an unexplored direction

				stack = rank;
			}
			
			//Move beyond current node
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
				nodeList[stack][NODEID_B] = INFINITY;
				break;
			case 1: //facing right: came from left
				nodeList[stack][NODEID_L] = INFINITY;
				break;
			case 2: //facing down: came from top
				nodeList[stack][NODEID_T] = INFINITY;
				break;
			case 3: //facing left: came from right
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
		}
		else if (wall)
		{
			//simLog("wall above");
			nodeCurrent[NODEID_T] = INFINITY;
		}
		nodeCurrent[DIST_T] = INFINITY;
		break;
	}
	case 1:
	{
		if (!wall)
		{
			nodeCurrent[NODEID_R] = 0;
		}
		else if (wall)
		{
			//simLog("wall right");
			nodeCurrent[NODEID_R] = INFINITY;
		}
		nodeCurrent[DIST_R] = INFINITY;
		break;
	}
	case 2:
	{
		if (!wall)
		{
			nodeCurrent[NODEID_B] = 0;
		}
		else if (wall)
		{
			//simLog("wall below");
			nodeCurrent[NODEID_B] = INFINITY;
		}
		nodeCurrent[DIST_B] = INFINITY;
		break;
	}
	case 3:
	{
		if (!wall)
		{
			nodeCurrent[NODEID_L] = 0;
		}
		else if (wall)
		{
			//simLog("wall left");
			nodeCurrent[NODEID_L] = INFINITY;
		}
		nodeCurrent[DIST_L] = INFINITY;
		break;
	}
	}
}

//nodeCurrent: current node, nodeStack: previous node, nodePrevious: previous node's nodeID, directionPrevious: previous node's exit direction, dist: distance between nodes
static bool addNodePath(short int direction, int nodeCurrent[DATA], int nodeStack[DATA], int nodePrevious, short int directionPrevious, int dist)
{
	fprintf(stderr, "adding nodePrevious (%d) as path connected to nodeCurrent (%d) %d apart.\n", nodePrevious, nodeCurrent[NODEID], dist);
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
		nodeStack[DIST_T] = dist;
		break;
	case 1:
		nodeStack[NODEID_R] = nodeCurrent[NODEID];
		nodeStack[DIST_R] = dist;
		break;
	case 2:
		nodeStack[NODEID_B] = nodeCurrent[NODEID];
		nodeStack[DIST_B] = dist;
		break;
	case 3:
		nodeStack[NODEID_L] = nodeCurrent[NODEID];
		nodeStack[DIST_L] = dist;
		break;
	}
	switch (direction) //add new path to nodeCurrent
	{
	case 0: //facing up; it's down
		nodeCurrent[NODEID_B] = nodePrevious;
		nodeCurrent[DIST_B] = dist;
		break;
	case 1: //facing right; it's left
		nodeCurrent[NODEID_L] = nodePrevious;
		nodeCurrent[DIST_L] = dist;
		break;
	case 2: //facing down; it's up
		nodeCurrent[NODEID_T] = nodePrevious;
		nodeCurrent[DIST_T] = dist;
		break;
	case 3: //facing left; it's right
		nodeCurrent[NODEID_R] = nodePrevious;
		nodeCurrent[DIST_R] = dist;
		break;
	}
	return true;
}
