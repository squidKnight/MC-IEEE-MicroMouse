/*
Written by Mathazzar
Last modified: 11/6/20
Purpose: take the path defined during the exploration phase and smooth it for optimized final traversal.
Status: FINISHED, TESTED
NOTE: currently one of several potential implementations of smoothing operations.
Probably needs to be rewritten to be more elegant/ not execute innefficient code.
Smoothing algorithm based on code in article at https://medium.com/@jaems33/understanding-robot-motion-path-smoothing-5970c8363bc4
*/

#include <stdio.h>
#include "mouseDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)

static int RATIO = 36; //ratio of side lengths from 16x16 grid to smoothed grid (currently 16x16-576x576, so 36:1)
static short int getYold(short int nodeID);
static short int getYtrans(short int nodeID);
static short int getXold(short int nodeID);
static short int getXtrans(short int nodeID);
static short int getIDtrans(short int nodeID);
static short int directionCheck(short int nodeCurrent, short int nodeNext);
static bool cornerCheck(short int nodeCurrent, short int nodeInterim, short int nodeNext);

//constants to be manually configured here based on physical micromouse dimensionsNOT CURRENTLY USED, NEED TO UNDERSTAND INPUTS BETTER FIRST
static short int ALPHA = 18;
static short int BETA = 2;
static short int TOLERANCE = 1;

/*void smootherV2(short int pathList[NODES / 2], short int smoothList[NODES][2])
INPUTS: short int pathList[NODES / 2], short int smoothList[NODES][0]
	pathList: the pathList array, storing the 16x16 grid node-to-node path.
	smoothList: blank array to be filled by smootherV2() with a more granular version of pathList using actual coords. smoothList[i][0] == x-coord, smoothList[i][1] == y-coord.
RETURNS: short int pathList[NODES / 2], short int smoothList[NODES][2]
	pathList: should return unmodified.
	smoothList: directly modified by smootherV2(), provides a more granular version of pathList with smoothed movements in a grid.
NOTES:
	Implemented to have smoothList array created beforehand, and assumes it to be blank and initializes it.
	The smoothList array actively breaks the nodeID up into its component x-y parts, because the grid has more spaces than can be fit in a positive signed short int without stack overflow.
		This effectively results in smoothList being a list of more granular versions of the position array.
	smootherV2 uses the corner-reducing method from smootherV0, then passes the diagonal-optimized data to the algorithm from smootherV1 for final processing.
CAUTION:
	Interacts with the pathList array passed to it directly.
	Manipulates the smoothList array passed to it directly.
*/
void smootherV2(short int pathList[NODES / 2], short int smoothList[NODES][2])
{
	simLog("Smoothing final path...V1");
	//initialize dataset
	for (int i = 0; i < NODES; i++)
	{
		smoothList[i][0] = INFINITY;
		smoothList[i][1] = INFINITY;
	}
	smoothList[0][0] = getXtrans(pathList[0]);
	smoothList[0][1] = getYtrans(pathList[0]);
	fprintf(stderr, "pathList[0] %d == smoothList[0] (%d, %d), smoothList[1] (", pathList[0], smoothList[0][0], smoothList[0][1]);
	fflush(stderr);
	switch (directionCheck(pathList[0], pathList[1]))
	{
	case 0:
		smoothList[1][0] = smoothList[0][0];
		smoothList[1][1] = smoothList[0][1] + (RATIO / 2);
		break;
	case 1:
		smoothList[1][0] = smoothList[0][0] + (RATIO / 2);
		smoothList[1][1] = smoothList[0][0];
		break;
	case 2:
		smoothList[1][0] = smoothList[0][0];
		smoothList[1][1] = smoothList[0][1] - (RATIO / 2);
		break;
	case 3:
		smoothList[1][0] = smoothList[0][0] - (RATIO / 2);
		smoothList[1][1] = smoothList[0][0];
		break;
	case -1:
		simLog("CRITICAL ERROR: pathList's next node doesn't appear to be adjacent to the current node.");
		fprintf(stderr, "smoothList[0] (%d, %d), smoothList[1] (%d, %d)\n", smoothList[0][0], smoothList[0][1], smoothList[1][0], smoothList[1][1]);
		fflush(stderr);
		break;
	}
	fprintf(stderr, "%d, %d)\n", smoothList[1][0], smoothList[1][1]);
	fflush(stderr);
	short int size = 0; //needed for later step
	for (int i = 1; i < NODES / 2; i++)
	{
		if (pathList[i] != INFINITY)
		{
			smoothList[i * 2][0] = getXtrans(pathList[i]);
			smoothList[i * 2][1] = getYtrans(pathList[i]);
			fprintf(stderr, "pathList[%d] %d == smoothList[%d] (%d, %d), smoothList[", i, pathList[i], i * 2, smoothList[i * 2][0], smoothList[i * 2][1]);
			fflush(stderr);
			size++;
			switch (directionCheck(pathList[i], pathList[i + 1]))
			{
			case 0:
				smoothList[i * 2 + 1][0] = smoothList[i * 2][0];
				smoothList[i * 2 + 1][1] = smoothList[i * 2][1] + (RATIO / 2);
				size++;
				break;
			case 1:
				smoothList[i * 2 + 1][0] = smoothList[i * 2][0] + (RATIO / 2);
				smoothList[i * 2 + 1][1] = smoothList[i * 2][1];
				size++;
				break;
			case 2:
				smoothList[i * 2 + 1][0] = smoothList[i * 2][0];
				smoothList[i * 2 + 1][1] = smoothList[i * 2][1] - (RATIO / 2);
				size++;
				break;
			case 3:
				smoothList[i * 2 + 1][0] = smoothList[i * 2][0] - (RATIO / 2);
				smoothList[i * 2 + 1][1] = smoothList[i * 2][1];
				size++;
				break;
			case -1:
				simLog("CRITICAL ERROR: pathList's next node doesn't appear to be adjacent to the current node.");
				fprintf(stderr, "smoothList[%d] (%d, %d), smoothList[%d] (%d, %d)\n", i * 2, smoothList[i * 2][0], smoothList[i * 2][1], i * 2 + 1, smoothList[i * 2 + 1][0], smoothList[i * 2 + 1][1]);
				fflush(stderr);
				break;
			}
			fprintf(stderr, "%d] (%d, %d)\n", i * 2 + 1, smoothList[i * 2 + 1][0], smoothList[i * 2 + 1][1]);
			fflush(stderr);
		}
	}
	//Verify copied path is the same
	for (int i = 15; i > -1; i--)
	{
		for (int j = 0; j < 16; j++)
		{
			bool exists = false;
			for (int k = 0; k < NODES / 2; k++)
			{
				if (pathList[k] == (i * 16) + j + 1)
				{
					exists = true;
					break;
				}
			}
			if (exists == true)
			{
				fprintf(stderr, "x ");
				fflush(stderr);
			}
			else
			{
				fprintf(stderr, "o ");
				fflush(stderr);
			}
		}
		fprintf(stderr, "\n");
		fflush(stderr);
	}
	for (int y = 576; y > -1; y--)
	{
		for (int x = 0; x < 577; x++)
		{
			bool exists = false;
			bool existsW = false;
			for (int k = 0; k < NODES; k++)
			{
				if (getXtrans(k) - (RATIO / 2) == x && getYtrans(k) - (RATIO / 2) == y)
				{
					existsW = true;
					break;
				}
				if (smoothList[k][0] == x && smoothList[k][1] == y)
				{
					exists = true;
					break;
				}
			}
			if (existsW)
			{
				fprintf(stderr, "+");
				fflush(stderr);
			}
			else if (exists)
			{
				fprintf(stderr, "x");
				fflush(stderr);
			}
			else
			{
				fprintf(stderr, "o");
				fflush(stderr);
			}
		}
		fprintf(stderr, "\n");
		fflush(stderr);
	}

	//Removing corners
	simLog("Optimizing diagonals...");
	//delete corners where possible
	for (int i = 0; i < (NODES / 2) - 1; i++)
	{
		if (pathList[i] == INFINITY)
			break;
		if (cornerCheck(pathList[i], pathList[i + 1], pathList[i + 2]))
		{
			fprintf(stderr, "nodeID %d/(%d, %d) is a corner(%d, %d), removing.\n", pathList[i + 1], smoothList[i * 2][0], smoothList[i * 2][1], pathList[i], pathList[i + 2]);
			fflush(stderr);
			smoothList[(i + 1) * 2][0] = INFINITY;
			smoothList[(i + 1) * 2][1] = INFINITY;
		}
	}
	//shift array left to fill empty spaces from corners being removed
	for (int i = 0; i < NODES - 1; i++)
	{
		if (smoothList[i][0] == INFINITY)
		{
			if (smoothList[i + 1][0] != INFINITY)
				for (int j = i; j < NODES; j++)
				{
					smoothList[i][0] = smoothList[i + 1][0];
					smoothList[i][1] = smoothList[i + 1][1];
				}
			else
				break;
		}
	}
	//Verify data integrity
	for (int y = 576; y > -1; y--)
	{
		for (int x = 0; x < 577; x++)
		{
			bool existsP = false;
			bool existsS = false;
			bool existsW = false;
			for (int k = 0; k < NODES; k++)
			{
				if (getXtrans(k) - (RATIO / 2) == x && getYtrans(k) - (RATIO / 2) == y)
				{
					existsW = true;
					break;
				}
				if (smoothList[k][0] == x && smoothList[k][1] == y)
				{
					existsP = true;
					break;
				}
			}
			if (existsW)
			{
				fprintf(stderr, "+");
				fflush(stderr);
			}
			else if (existsP)
			{
				fprintf(stderr, "Q");
				fflush(stderr);
			}
			else
			{
				fprintf(stderr, "o");
				fflush(stderr);
			}
		}
		fprintf(stderr, "\n");
		fflush(stderr);
	}

	//Smooth path
	simLog("Optimizing  overall path...");
	//deep copy of smoothList
	short int tempList[NODES][2];
	for (int i = 0; i < NODES; i++)
	{
		tempList[i][0] = smoothList[i][0];
		tempList[i][1] = smoothList[i][1];
	}
	//Iterative check of each possible change
	short int change = TOLERANCE;
	while (change >= 0.000001)
	{
		change = 0;
		for (int i = 1; i < size; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				short int x_i = smoothList[i][j];
				short int y_i = tempList[i][j];
				short int y_prev = tempList[i - 1][j];
				short int y_next = tempList[i + 1][j];
				short int y_i_saved = y_i;
				y_i += 0.1 * (x_i - y_i) + 0.1 * (y_next + y_prev - (2 * y_i));
				fprintf(stderr, "Checking (%d, %d) as (%d, %d)\n", smoothList[i][0], smoothList[i][1], tempList[i][0], tempList[i][1]);
				fflush(stderr);
				tempList[i][j] = y_i;
				change += abs(y_i - y_i_saved);
			}
		}
	}
	//copy tempList back to smoothList
	/*for (int i = 0; i < NODES / 2; i++)
	{
	fprintf(stderr, "smoothList[%d] updated from (%d, %d) to (%d, %d)\n", i, smoothList[i][0], smoothList[i][1], tempList[i][0], tempList[i][1]);
	fflush(stderr);
	smoothList[i][0] = tempList[i][0];
	smoothList[i][1] = tempList[i][1];
	}*/
	//Verify data integrity
	for (int y = 576; y > -1; y--)
	{
		for (int x = 0; x < 577; x++)
		{
			bool existsP = false;
			bool existsS = false;
			bool existsW = false;
			for (int k = 0; k < NODES; k++)
			{
				if (getXtrans(k) - (RATIO / 2) == x && getYtrans(k) - (RATIO / 2) == y)
				{
					existsW = true;
					break;
				}
				if (smoothList[k][0] == x && smoothList[k][1] == y)
				{
					existsP = true;
					break;
				}
				if (tempList[k][0] == x && tempList[k][1] == y)
				{
					existsS = true;
					break;
				}
			}
			if (existsW)
			{
				fprintf(stderr, "+");
				fflush(stderr);
			}
			else if (existsS)
			{
				fprintf(stderr, "x");
				fflush(stderr);
			}
			else if (existsP)
			{
				fprintf(stderr, "Q");
				fflush(stderr);
			}
			else
			{
				fprintf(stderr, "o");
				fflush(stderr);
			}
		}
		fprintf(stderr, "\n");
		fflush(stderr);
	}
}

static short int getYold(short int nodeID)
{
	return (nodeID - 1) / 16;
}

static short int getYtrans(short int nodeID)
{
	return getYold(nodeID) * RATIO + (RATIO / 2);
}

static short int getXold(short int nodeID)
{
	return (nodeID - 1) - (getYold(nodeID) * 16);
}

static short int getXtrans(short int nodeID)
{
	return getXold(nodeID) * RATIO + (RATIO / 2);
}

static short int getIDtrans(short int nodeID)
{
	return (getYtrans(nodeID) * 576) + getXtrans(nodeID) + 577;
}

static short int directionCheck(short int nodeCurrent, short int nodeNext)
{
	if (nodeNext == nodeCurrent + 16)
		return 0;
	else if (nodeNext == nodeCurrent + 1)
		return 1;
	else if (nodeNext == nodeCurrent - 16)
		return 2;
	else if (nodeNext == nodeCurrent - 1)
		return 3;
	else
		return -1;
}

static bool cornerCheck(short int nodeCurrent, short int nodeInterim, short int nodeNext)
{
	if (nodeInterim == nodeCurrent + 16 || nodeInterim == nodeCurrent - 16)
	{
		if (nodeNext == nodeInterim + 1 || nodeNext == nodeInterim - 1)
		{
			simLog("NE");
			return true;
		}
		else
			return false;
	}
	else if (nodeInterim == nodeCurrent + 1 || nodeInterim == nodeCurrent - 1)
	{
		if (nodeNext == nodeInterim + 16 || nodeNext == nodeInterim - 16)
		{
			simLog("SW");
			return true;
		}
		else
			return false;
	}
	else
	{
		simLog("ERROR: nodeCurrent and nodeInterim don't appear to be adjacent.");
		return false;
	}
}
