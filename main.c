/*
Written by squidKnight, Mathazzar
Last modified: 06/04/20
Purpose: run the program on the simulator, which requires a main.c

NOTE: ONLY USED FOR SIMULATOR, NOT FOR USE ON ARDUINO!!
*/

#include "mouseDefs.h"
#include <stdio.h>

int nodeList[NODES][DATA];

void nodeInit(int nodeList[NODES][DATA]); //initialize nodeList
void scan(int nodeList[NODES][DATA]); //scans the maze

int main()
{
	nodeInit(nodeList);
	scan(nodeList);
	for (int i = 0; i < NODES; i++)
	{
		fprintf(stderr, "%d;\t%d, %d, %d, %d;\t%d, %d, %d, %d\n", i, nodeList[i][NODEID], nodeList[i][NODEID_T], nodeList[i][NODEID_R], nodeList[i][NODEID_B], nodeList[i][NODEID_L], nodeList[i][DIST_T], nodeList[i][DIST_R], nodeList[i][DIST_B], nodeList[i][DIST_L]);
		fflush(stderr);
	}
	return 0;
}
