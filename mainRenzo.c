#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "API.h"

void nodeInitialization(void);
void scan(void);
int nodeClassification(int NIN, int PathList[][3]);
int choice(int NIN);
void choseForward(void);
void choseRight(void);
void turnRight(void);
void choseLeft(void);
void turnLeft(void);
int wallFront(void);
int wallRight(void);
int wallLeft(void);
int calculate_heuristic_distance(int NIN);

#define INF 1024
#define NOPATHS 255
#define WALL 9
#define MAXDIST 14

int NodeList[256][5]; /*Node Identification Numbers: 1 - 256*/ /*Heuristic Distance*/ /*Backpath*/ /*0||1*/ /*PathList*/

int PathList[256][3]; /*Front path*/ /*Right path*/ /*Left path*/ /*NIN 0 is used to distinguish NIN from no path like INF is used to distinguish nodes and unknown nodes*/

int orientation = 0; /*0 Up, 1 Down, 2 Right 3 Left*/
int stepDistance = 0;
int NIN = 1;
int endGoal = 256;
int distanceToGoal;
int nodeClass;

char Q3[] = "1a2a3a4a5a6a7a8a17a18a19a20a21a22a23a24a33a34a35a36a37a38a39a40a49a50a51a52a53a54a55a56a65a66a67a68a69a70a71a72a81a82a83a84a85a86a87a88a97a98a99a100a101a102a103a104";
char Q4[] = "9a10a11a12a13a14a15a16a25a26a27a28a29a30a31a32a41a42a43a44a45a46a47a48a57a58a59a60a61a62a63a64a73a74a75a76a77a78a79a80a89a90a91a92a93a94a95a96a105a106a107a108a109a110a111a112a";
char Q2[] = "129a130a131a132a133a134a135a145a146a147a148a149a150a151a152a161a162a163a164a165a166a167a168a177a178a179a180a181a182a183a184a193a194a195a196a197a198a199a200a209a210a211a212a213a214a215a216a225a226a227a228a229a230a231a232a241a242a243a244a245a246a247a248a";
char Q1[] = "138a139a140a141a142a143a144a153a154a155a156a157a158a159a160a169a170a171a172a173a174a175a176a185a186a187a188a189a190a191a192a201a202a203a204a205a206a207a208a217a218a219a220a221a222a223a224a233a234a235a236a237a238a239a240a249a250a251a252a253a254a255a256a";

int main()
{
	simLog("/*Start*/");
	nodeInitialization();
	scan();
}//end main

//Initializes node list
void nodeInitialization()
{
	int i;
	for (i = 0; i < 256; i++)
	{
		NodeList[i][0] = i + 1;//
		NodeList[i][1] = INF;
		NodeList[i][2] = 0;
		NodeList[i][3] = 0;
		NodeList[i][4] = NOPATHS;
	}
	
	API_setColor(0,0,'Y'); //visual mark
	API_setText(0, 0, "start");
	API_setColor(7,7,'Y');
	API_setColor(7,8,'Y');
	API_setColor(8,8,'Y');
	API_setColor(8,7,'Y');
	API_setText(7, 7, "Goal");
	API_setText(7, 8, "Goal");
	API_setText(8, 8, "Goal");
	API_setText(8, 7, "Goal");

	/*Node Identification Number*/ /*Heuristic Distance*/ /*Backpath*/ /*0||1*/ /*PathList*/
	NodeList[1][0] = 1;//
	NodeList[1][1] = MAXDIST; /*7 inward + 7 inward*/
	NodeList[1][2] = 1; /*NULL == 0*/
	NodeList[1][3] = 1; /*0 = Corner/Unexplorable || 1 = Explorable*/ //non-explorable //hmm 0 or 1 at S depends on maze and affects main()
	NodeList[1][4] = 0;/*PathList[0]*/
}//end initialization
//confused Nodelist priority queue

//Scans the maze
void scan()
{
	/*S*/
	nodeClass = nodeClassification(1, PathList[1]);

	//while (NIN != 120 && NIN != 121 && NIN != 136 && NIN != 137)
	int i;
	while (i < 10)
	{
		if (nodeClass == 0)
        	{
               		choseForward();
               	 	nodeClass = nodeClassification(NIN, PathList[NIN]);
		}
		else if (nodeClass == 2)
		{
			if (PathList[NIN][3] != 0)
			{
				choseRight();
				nodeClass = nodeClassification(NIN, PathList[NIN]);
			}
			else if (PathList[NIN][2] != 0)
			{
				choseLeft();
				nodeClass = nodeClassification(NIN, PathList[NIN]);
			}
		}
		else if (nodeClass == 1)
		{
			simLog("This is a node.\n");
			choice(NIN);
		}
		else if (nodeClass == -1)
		{
			//what about /*explore all options
			returnToNode();
		}
		i++;
	}	
}//end scan

	/*-1 = Deadend || 0 = Clear Path || 1 = Explorable Node || 2 = Corner/Unexplorable Node*/
//Classifies every node
int nodeClassification(int NIN, int PathList[NIN][3])
{
	int pathsAvailable = 3;

	/*Classify node by available paths*/
	if (orientation == 0)
	{
		if (wallFront()) /*Front*/
		{
			pathsAvailable--;
		}
		else
		{
			PathList[NIN][0] = NIN + 16;
		}
		if (wallRight()) /*Right*/
		{
			pathsAvailable--;
		}
		else
		{
			PathList[NIN][1] = NIN + 1;
		}
		if (wallLeft()) /*Left*/
		{
			pathsAvailable--;
		}
		else
		{
			PathList[NIN][2] = NIN - 1;
		}
	}
	else if (orientation == 1)
	{
		if (wallFront()) /*Front*/
                {
                        pathsAvailable--;
                }
                else
                {
                        PathList[NIN][0] = NIN - 16;
                }
                if (wallRight()) /*Right*/
                {
                        pathsAvailable--;
                }
                else
                {
                        PathList[NIN][1] = NIN - 1;
                }

                if (wallLeft()) /*Left*/
                {
                        pathsAvailable--;
                }
                else
                {
                        PathList[NIN][2] = NIN + 1;
                }

	}
	else if (orientation == 2)
        {
                if (wallFront()) /*Front*/
                {
                        pathsAvailable--;
                }
                else
                {
                        PathList[NIN][0] = NIN + 1;
                }
                if (wallRight()) /*Right*/
                {
                        pathsAvailable--;
                }
                else
                {
                        PathList[NIN][1] = NIN - 16;
                }

                if (wallLeft()) /*Left*/
                {
                        pathsAvailable--;
                }
                else
                {
                        PathList[NIN][2] = NIN + 16;
                }
        }
        else if (orientation == 3)
        {
                if (wallFront()) /*Front*/
                {
                        pathsAvailable--;
                }
                else
                {
                        PathList[NIN][0] = NIN - 1;
                }
                if (wallRight()) /*Right*/
                {
                        pathsAvailable--;
                }
                else
                {
                        PathList[NIN][1] = NIN + 16;
                }
                if (wallLeft()) /*Left*/
                {
                        pathsAvailable--;
                }
                else
                {
                        PathList[NIN][2] = NIN - 1;
                }
        }

	switch (pathsAvailable) /*Return classification of node*/
	{
		case 0: simLog("Deadend\n");
			return -1; /*Deadend*/
			break;
		case 1: /*Path*/
			if (API_wallFront())
			{	simLog("Corner\n");
				return 2; /*Corner*/
			}
			else
			{
				simLog("Straight path\n");
				return 0;
			}
			break;
		case 2: simLog("Node\n");
			return 1; /*Explorable Node*/
			break;
		case 3: simLog("Node\n");
			return 1; /*Explorable Node*/
			break;
		default: return -2;
			break;
	}
}//end classification

/*Choice*/
int choice(int NIN)
{
	int i;
	int min = 0;
	int minimum = MAXDIST;
	int choices = 0;
	int option1 = -1;
	int option2 = -1;
	int orientationoption1 = -1;
	int orientationoption2 = -1;
	int optimalchoice = -1;
	int suboptimalchoice = -1;
	int leastoptimalchoice = -1;

	for (i = 0; i < 2; i++) /*Heuristic Remaining Distance to Goal*/
	{
		if (PathList[NIN][i] != 0) /*Adjacent accessible coordinate*/
		{
			min = calculate_heuristic_distance(PathList[NIN][i]); //Calculate heuristic remaining distance

			if (min < minimum) /*Optimal choice*/
			{
				minimum = min;
				leastoptimalchoice = suboptimalchoice;
				suboptimalchoice = optimalchoice;
				optimalchoice = i;

				if (PathList[NIN][i] == NIN + 16)
				{
					orientationoption1 = 0;
					option1 = i;
				}
				else if (PathList[NIN][i] == NIN - 16)
				{
					orientationoption1 = 1;
					option1 = i;
				}
				else if (PathList[NIN][i] == NIN + 1)
				{
					orientationoption1 = 2;
					option1 = i;
				}
				else if (PathList[NIN][i] == NIN - 1)
				{
					orientationoption1 = 3;
					option1 = i;
				}
			}

			else if (min == minimum) /*Equidistant paths evaluated based on orientation*/
			{
				if (PathList[NIN][i] == NIN + 16)
				{
					orientationoption2 = 0;
					option2 = i;
				}
				else if (PathList[NIN][i] == NIN - 16)
				{
					orientationoption2 == 1;
					option2 = i;
				}
				else if (PathList[NIN][i] == NIN + 1)
				{
					orientationoption2 == 2;
					option2 = i;
				}
				else if (PathList[NIN][i] == NIN - 1);
				{
					orientationoption2 = 3;
					option2 = i;
				}
				leastoptimalchoice = suboptimalchoice;
				if (orientation == orientationoption1)
				{
					optimalchoice = option1;
					suboptimalchoice = option2;
				}
				else if (orientation == orientationoption2)
				{
					optimalchoice = option2;
					suboptimalchoice = option1;
				}
			}

			else /*Suboptimal and least optimal choice*/
			{
				if (suboptimalchoice != -1)
				{
					leastoptimalchoice = i; //not evaluating orientation not in the mood
				}
				else
				{
					suboptimalchoice = i;
				}
			}

		}
	}//optimal choice


	/*Count choices*/
	if (suboptimalchoice != -1)
	{
		choices++;
	}
	if (leastoptimalchoice != -1)
	{
		choices++;
	}

	/*Explore all choices*/
	for (i = 0; i < choices; i++)
	{

	}
	if (optimalchoice == 0)
	{
		choseForward();
	}
	else if (optimalchoice == 1)
	{
		choseRight();
	}
	else if (optimalchoice == 2)
	{
		choseLeft();
	}

	//after it explores all 3

	//choose [h]

	//after [h] return to main	
	
}//end choice

/*Movement*/

void returnToNode()
{
	API_turnRight();
	API_turnRight();

	while (nodeClass != 1)
	{
		choseForward();
		nodeClass = nodeClassification(NIN, PathList[NIN]);
	}
}

void choseForward()
{
	simLog("Moving forward\n");
	if (orientation == 0)
	{
		NIN += 16;
	}
	else if (orientation == 1)
	{
		NIN -= 16;
	}
	else if (orientation == 2)
	{
		NIN += 1;
	}
	else if (orientation == 3)
	{
		NIN -= 1;
	}
	API_moveForward();

	//while node
	//motorR(1), motorL(1);
	//turnleft
	//turnright
	//distancefromS++;
	
}//end forward movement

void choseRight()
{
	simLog("Turning right\n");
	turnRight(); //add a weight to dist
	if (orientation == 0) /*Maintain orientation*/
	{
		orientation = 2;
	}
	else if (orientation == 1)
	{
		orientation = 3;
	}
	else if (orientation == 2)
	{
		orientation = 1;
	}
	else if (orientation == 3)
	{
		orientation = 0;
	}
	choseForward();
}
void turnRight() //int deg for diagonals. void for now
{
	API_turnRight();

	//if (deg == 45)
	//motorR(-.5), motorL(.5); //#define HALF .5
	//if (deg == 90)
	//motorR(-1), motorL(1); //#define FULL 1
	//distancefromS++ sqrt2
	
}//end right turn

void choseLeft()
{
	simLog("Turning left\n");
	turnLeft();
	if (orientation == 0) /*Maintain orientation*/
	{
		orientation = 3;
	}
	else if (orientation == 1)
	{
		orientation = 2;
	}
	else if (orientation == 2)
	{
		orientation = 0;
	}
	else if (orientation == 3)
	{
		orientation = 1;
	}
	choseForward();

}

void turnLeft(void) //int deg for diagonals. void for now
{
	API_turnLeft();
	//if (deg == 45)
	//motorR(.5), motorL(-.5); //#define HALF .5
	//if (deg == 90)
	//motorR(-1), motorL(1); //#define FULL 1
	//distancefromS++ sqrt2
}//end left turn

/*Heuristic*/
int calculate_heuristic_distance(int NIN)
{
	char NINstr[5];
	snprintf(NINstr, 5,"%da",NIN);
	//itoa(NIN, NINstr, 5);
	int distanceToGoal = 0;
	int goal;
	//strcat(NINstr, "a");

	if (strcmp(NINstr, Q3))
	{
		goal = 120 - NIN;
		while (goal >= 16)
		{
			distanceToGoal++;
			goal -= 16;
		}
		while (goal > 0)
		{
			distanceToGoal++;
			goal--;
		}
		return distanceToGoal;
	}
	else if (strcmp(NINstr, Q4))
	{
		goal = 121 - NIN;
		while (goal > 0)
		{
			distanceToGoal++;
			goal -= 16;
		}
		while (goal < 0)
		{
			distanceToGoal++;
			goal++;
		}
		return distanceToGoal;
	}
	else if (strcmp(NINstr, Q2))
	{
		goal = 136 - NIN;
		while (goal <= -16 || goal < 0)
		{
			distanceToGoal++;
			goal += 16;
		}
		while (goal > 0)
		{
			distanceToGoal++;
			goal--;
		}
		return distanceToGoal;
	}
	else if (strcmp(NINstr, Q1))
	{
		goal = 137 - NIN;
		while (goal <= -16)
		{
			distanceToGoal++;
			goal += 16;
		}
		while (goal < 0)
		{
			distanceToGoal++;
			goal++;
		}
		return distanceToGoal;
	}
}//end heuristic distance

/*Sensor*/
//int sensorFront;
int wallFront()
{
	return API_wallFront();
	//int walldist = (IRV / analogRead(sensorFront));
	//if (walldist > WALL)
	//{
	//	return 1; //using 1 instead of 0
	//}
	//else
	//{
	//	return 0; //using 0 instead of 1
	//}
}//end front wall sensor

//int sensorRight;
int wallRight()
{
	return API_wallRight();
	//int walldist = (IRV / analogRead(sensorRight));
	//if (walldist > WALL)
	//{
	//	return 1; //using 1 instead of 0
	//}
	//else
	//{
	//	return 0; //using 0 instead of 1
	//}
}//end right wall sensor

//int sensorLeft;
int wallLeft()
{
	return API_wallLeft();

	//int walldist = (IRV / analogRead(sensorLeft));
	//if (walldist > WALL)
	//{
	//	return 1; //using 1 instead of 0
	//}
	//else
	//{
	//	return 0; //using 0 instead of 1
	//}
	
} //end left wall sensor

/*End Sensor*/
















//if (nodeClass == -1) //if deadend
//
//{
//
//	simLog("\t\tNode class: Dead-end\n\t\tReturning to previous node...");
//
//	API_setColor(position[0], position[1], 'R');
//
//}
//
//if (nodeClass == 1) //if maze node
//
//{
//
//	simLog("\t\tNode class: Path node\n\t\tRecording node information...");
//
//	API_setColor(position[0], position[1], 'B');
//
//
//
//	int nodeCurrent[4] = {}; //stores all information on current node
//
//	nodeCurrent[0] = nodeID; //node ID
//
//	nodeCurrent[1] = dist; //distance traveled
//
//	nodeCurrent[2] = nodePrevious; //backpath
//
//	nodeCurrent[3] = 1; //is an explorable node
//
//	nodePrevious = nodeID; //current node will be the next one's backpath
//
//	stackInsert(nodeCurrent); //inserts the node into the stack
//
//}
//
//if (nodeClass == 2) //if corner
//
//{
//
//	simLog("\t\tNode class: Corner\n\t\tRecording node information...");
//
//	API_setColor(position[0], position[1], 'G');
//
//	int nodeCurrent[4] = {};
//
//	nodeCurrent[0] = nodeID;
//
//	nodeCurrent[1] = dist;
//
//	nodeCurrent[2] = nodePrevious;
//
//	nodeCurrent[3] = 0; //is NOT an explorable node
//
//	nodePrevious = nodeID;
//
//	stackInsert(nodeCurrent);
//
//}
//
//
//int translate(int x, int y) //translates values from cartesian system used in simulator to 16x16 matrix node IDs
//
//{
//
//	int nodeID = (x + (y * 16) + 1); //conversion from cartesian to matrix
//
//	fprintf(stderr, "\t\tNode ID: %d\n", nodeID);
//
//	fflush(stderr);
//
//	return nodeID;
//
//}
//
//
//
//int getID(int direction, int dist, int position[2])
//
//{
//
//	//determine change based on direction
//
//	if (direction == 0) //if facing up
//
//		position[1] += dist; //y position increased (normally, this would be incremented in accordance to the matrix by recorded traveled distance)
//
//	if (direction == 1) //if facing down
//
//		position[1] -= dist;
//
//	if (direction == 2) //if facing right
//
//		position[0] += dist; //x position increased
//
//	if (direction == 3) //if facing left
//
//		position[0] -= dist;
//
//
//
//	//translate to matrix format, this can be removed once outside of simulator
//
//	return translate(position[0], position[1]);
//
//}
