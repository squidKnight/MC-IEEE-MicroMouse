#define INF 1024
#define NOPATHS 255
#define WALL 9
#define MAXDIST 14

int NodeList[256][5]; /*Node Identification Numbers: 1 - 256*/ /*Heuristic Distance*/ /*Backpath*/ /*0||1*/ /*PathList*/

int PathList[256][3]; /*Front path*/ /*Right path*/ /*Left path*/ /*NIN 0 is used to distinguish NIN from no path like INF is used to distinguish nodes and unknown nodes*/

int orientation = 0; /*0 Up, 1 Down, 2 Right 3 Left*/

char Q1[1000] = "1a2a";
char Q2[1000] = "";
char Q3[1000] = "";
char Q4[1000] = "";

//Motherboard
int main()
{
	nodeInitialization();
	scan();
}

//Initializes node list
void nodeInitialization()
{
	int i;
	for (i = 0; i < 256; i++)
	{
		NodeList[i][0] = i + 1;//
		NodeList[i][1] = INF;
		//Backpath = 0
		//Explorable/Not = 0
		NodeList[i][4] = NOPATHS;
	}

	/*Node Identification Number*/ /*Heuristic Distance*/ /*Backpath*/ /*0||1*/ /*PathList*/
	NodeList[0][0] = 1;//
	NodeList[0][1] = MAXDIST; /*7 inward + 7 inward*/
	NodeList[0][2] = 1; /*NULL == 0*/
	NodeList[0][3] = 1; /*0 = Corner/Unexplorable || 1 = Explorable*/ //non-explorable
	NodeList[0][4] = 0;/*PathList[0]*/
}//end initialization
//confused Nodelist priority queue

//Scans the maze
void scan()
{
	int nextmove; //First move

	/*S*/
	int nodeClass = nodeClassification(1, PathList[1]); //1: StackInsert
	NIN = 1;

	/*Choice*/
	while (NIN != endGoal)
	choice(NIN); //StackInsert //S B A //2

		   //while not node move forward and navigate corners
		   //gather options, place node in priority queue
		   //return() //orientation tricky   	 		 /*StackInsert*and then 0/1 return()*/ /*explore all options*/
		   //choose node [h] //3: 
		   //arrive at node NIN = NodeID
		   //while (NIN != endGoal)


	//while (nodeClass == 0) //waits until there is a node detected
	//{
	//	API_moveForward();
	//	dist++; //this needs to be replaced with motor functions to determie wall lengths traveled
	//	nodeClass = nodeCheck();
	//}
	//int nodeID = getID(direction, dist, position);

				  // Node B repeat path choice /*Deadend or node-to-node*/ pick one
				//while not node move forward and navigate corners
			//gather options, place node in priority queue
		   //return /*explore all options*/
		   //choose node [h]
		   //arrive at node

		   //END

	//Classification
	//choice
	//[h] until END

	//

	/*Orientation*/

	//to next node.

		/*Cont. Scan*/
		while (nodeClass == 0)
		{
			moveForward();
			stepDistance++;
			nodeClass = nodeClassification(NIN, PathList[NIN]);
		}
}//end scan

	/*-1 = Deadend || 0 = Clear Path || 1 = Explorable Node || 2 = Corner/Unexplorable Node*/
//Classifies every node
int nodeClassification(int NIN, int PathList[])
{
	int class, pathsAvailable = 3;

	/*Classify node by available paths*/
	if (wallFront()) /*Front*/
	{
		pathsAvailable--;
	}
	else
	{
		PathList[0] = NIN + 16;
	}

	if (wallRight()) /*Right*/
	{
		pathsAvailable--;
	}
	else
	{
		PathList[1] = NIN + 1;
	}

	if (wallLeft()) /*Left*/
	{
		pathsAvailable--;
	}
	else
	{
		PathList[2] = NIN - 1;
	}

	switch (pathsAvailable) /*Return classification of node*/
	{
	case 0: return class = -1; /*Deadend*/
		break;
	case 1: class = 0; /*Clear Path*/
		if (wallFront())
		{
			return class = 2; /*Corner/Unexplorable Node*/
		}
		break;
	case 2:
	case 3: return class = 1; /*Explorable Node*/
		break;
	default: return class = -2; /*Should never happen*/
		break;
	}
}//end classification

/*Choice*/
int choice(int NIN)
{
	int i;
	int min;
	int minimum = MAXDIST;
	int choices;
	int option1;
	int option2;
	int orientationoption1;
	int orientationoption2;
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
//void return()
//{
//	;
//}

void choseForward()
{
	API_moveForward();
	//while node
	//motorR(1), motorL(1);
	//turnleft
	//turnright
	//distancefromS++;
}//end forward movement

void choseRight()
{
	turnRight();
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
	;
}//end right turn

void choseLeft()
{
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

void turnLeft(int deg)
{
	API_turnLeft();
	//if (deg == 45)
	//motorR(.5), motorL(-.5); //#define HALF .5
	//if (deg == 90)
	//motorR(-1), motorL(1); //#define FULL 1
	//distancefromS++ sqrt2
	;
}//end left turn

/*Heuristic*/
int calculuate_heuristic_distance(int NIN)
{
	char NINstr[1000] = itoa(NIN);
	int distanceToGoal;
	int goal;

	strcat(NINstr, "a");

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
int sensorFront;
int wallFront()
{
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

int sensorRight;
int wallRight()
{
	API_wallRight();
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

int sensorLeft;
int wallFront()
{
	API_wallLeft();
	//int walldist = (IRV / analogRead(sensorLeft));
	//if (walldist > WALL)
	//{
	//	return 1; //using 1 instead of 0
	//}
	//else
	//{
	//	return 0; //using 0 instead of 1
	//}
}//end left wall sensor
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
