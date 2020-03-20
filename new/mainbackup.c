#define INF 1024
#define NOPATHS 255
#define WALL 9
#define MAXDIST 14

int NodeList[256][5]; /*Node Identification Numbers: 1 - 256*/ /*Heuristic Distance*/ /*Backpath*/ /*0||1*/ /*PathList*/

int PathList[256][3]; /*Front path*/ /*Right path*/ /*Left path*/ /*NIN 0 is used to distinguish NIN from no path like INF is used to distinguish nodes and unknown nodes*/

int orientation = 0; /*0 Up, 1 Down, 2 Right 3 Left*/

char Q1 = "1a2a";
char Q2 = "";
char Q3 = "";
char Q4 = "";

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
		NodeList[i][0] = i+1;//
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
	int nodeClass = nodeClassification(1, PathList[1]);

	/*Choice*/
	choice(1); //while not node move forward and navigate corners
		   //gather options, place node in priority queue
		   //return() //orientation tricky   	 		  /*explore all options*/
		   //choose node [h]
		   //arrive at node
		   
		   // Node B repeat path choice /*Deadend or node-to-node*/
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
	
	to next node.

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
	int minimum = MAXDIST;
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
			min = calculate_heuristic_distance(Pathlist[NIN][i]); //Calculate heuristic remaining distance
		
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
                                   	oritentationoption1 = 1;
                                        option1 = i;
                               	}
				else if (PathList[NIN][i] == NIN + 1)
                                {
                                        orientationoption1 = 2;
                                        option1 = i;
                                }
                                else if (PathList[NIN][i] == NIN - 1)
                                {
                                        oritentationoption1 = 3;
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
void return()
{
	
}

void choseForward()
{
	//while node
	motorR(1), motorL(1);
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
	else if (orienation == 1)
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
void turnRight(int deg)
{	
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
        else if (orienation == 1)
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
	int distanceToGoal;
	int goal;

	NINstr = itoa(NIN);
	NINstr = strcat(NIN, "a");
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
	if (strcmp(NINstr, Q4))
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
	if (strcmp(NINstr, Q2))
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
	if (strcmp(NINstr, Q1))
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
	int walldist = (IRV / analogRead(sensorFront));
	if (walldist > WALL)
	{
		return 1; //using 1 instead of 0
	}
	else
	{
		return 0; //using 0 instead of 1
	}
}//end front wall sensor

int sensorRight;
int wallRight()
{
        int walldist = (IRV / analogRead(sensorRight));
        if (walldist > WALL)
        {
                return 1; //using 1 instead of 0
        }
        else
        {
                return 0; //using 0 instead of 1
        }
}//end right wall sensor

int sensorLeft;
int wallFront()
{
        int walldist = (IRV / analogRead(sensorLeft));
        if (walldist > WALL)
        {
                return 1; //using 1 instead of 0
        }
        else
        {
                return 0; //using 0 instead of 1
        }
}//end left wall sensor
/*End Sensor*/


