/*
Written by Mathazzar
Last modified: 10/27/20
Purpose: take the path defined during the exploration phase and smooth it for optimized final traversal.
Status: NOT FINISHED, TESTED
NOTE: currently one of several potential implementations of smoothing operations.
	Probably needs to be rewritten to be more elegant/ not execute innefficient code.
*/

#include <stdio.h>
#include "mouseDefs.h"

void simLog(char* text); //modified from main.c in mms example (https://github.com/mackorone/mms-c)

static int RATIO = 2; //ratio of side lengths from 16x16 grid to smoothed grid (currently 16x16-33x33, so 2:1)
static short int getYold(short int nodeID);
static short int getYtrans(short int nodeID);
static short int getXold(short int nodeID);
static short int getXtrans(short int nodeID);
static short int getIDtrans(short int nodeID);
static short int directionCheck(short int nodeCurrent, short int nodeNext);

/*void smootherV0(short int pathList[NODES / 2], short int smoothList[NODES * 2])
INPUTS: short int pathList[NODES / 2], short int smoothList[NODES * 2]
	pathList: the pathList array, storing the 16x16 grid node-to-node path.
	smoothList: blank array to be filled by smootherV0() with a more granular version of pathList.
RETURNS: short int pathList[NODES / 2], short int smoothList[NODES * 2]
	pathList: should return unmodified.
	smoothList: directly modified by smootherV0(), provides a more granular version of pathList with smoothed movements.
NOTES:
	Implemented to have smoothList array created beforehand, and assumes it to be blank and initializes it.
CAUTION:
	Interacts with the pathList array passed to it directly.
	Manipulates the smoothList array passed to it directly.
*/
void smootherV0(short int pathList[NODES / 2], short int smoothList[NODES * 2])
{
	simLog("Smoothing final path...");
	//initialize dataset
	for (int i = 0; i < NODES * 2; i++)
	{
		smoothList[i] = INFINITY * 2;
	}
	smoothList[0] = getIDtrans(pathList[0]);
	fprintf(stderr, "pathList[0] %d == smoothList[0] %d\n", pathList[0], smoothList[0]);
	fflush(stderr);
	switch (directionCheck(pathList[0], pathList[1]))
	{
	case 0:
		smoothList[1] = smoothList[0] + 32;
		break;
	case 1:
		smoothList[1] = smoothList[0] + 1;
		break;
	case 2:
		smoothList[1] = smoothList[0] - 32;
		break;
	case 3:
		smoothList[1] = smoothList[0] - 1;
		break;
	case -1:
		simLog("CRITICAL ERROR: pathList's next node doesn't appear to be adjacent to the current node.");
		fprintf(stderr, "smoothList[0] %d, smoothList[1] %d\n", smoothList[0], smoothList[1]);
		fflush(stderr);
		break;
	}
	fprintf(stderr, ", smoothList[%d] %d\n", 1, smoothList[1]);
	fflush(stderr);
	for (int i = 1; i < NODES / 2; i++)
	{
		if (pathList[i] != INFINITY)
		{
			smoothList[i * RATIO] = getIDtrans(pathList[i]);
			fprintf(stderr, "pathList[%d] %d == smoothList[%d] %d", i, pathList[i], i * RATIO, smoothList[i * RATIO]);
			fflush(stderr);

			//add connector
			switch (directionCheck(pathList[i], pathList[i + 1]))
			{
			case 0:
				smoothList[i * RATIO + 1] = smoothList[i * RATIO] + 32;
				break;
			case 1:
				smoothList[i * RATIO + 1] = smoothList[i * RATIO] + 1;
				break;
			case 2:
				smoothList[i * RATIO + 1] = smoothList[i * RATIO] - 32;
				break;
			case 3:
				smoothList[i * RATIO + 1] = smoothList[i * RATIO] - 1;
				break;
			case -1:
				simLog("CRITICAL ERROR: pathList's next node doesn't appear to be adjacent to the current node.");
				fprintf(stderr, "smoothList[%d] %d, smoothList[%d] %d\n", i * RATIO, smoothList[i * RATIO], i * RATIO + 1, smoothList[i * RATIO + 1]);
				fflush(stderr);
				break;
			}
			fprintf(stderr, ", smoothList[%d] %d\n", i * RATIO + 1, smoothList[i * RATIO + 1]);
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
	for (int i = 32; i > -1; i--)
	{
		for (int j = 0; j < 33; j++)
		{
			bool exists = false;
			for (int k = 0; k < NODES * 2; k++)
			{
				if (smoothList[k] == (i * 32) + j)
				{
					exists = true;
					break;
				}
			}
			if (exists)
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
}

static short int getYold(short int nodeID)
{
	return (nodeID - 1) / 16;
}

static short int getYtrans(short int nodeID)
{
	return getYold(nodeID) * RATIO;
}

static short int getXold(short int nodeID)
{
	return (nodeID - 1) - (getYold(nodeID) * 16);
}

static short int getXtrans(short int nodeID)
{
	return getXold(nodeID) * RATIO;
}

static short int getIDtrans(short int nodeID)
{
	return (getYtrans(nodeID) * 32) + getXtrans(nodeID) + 33;
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
