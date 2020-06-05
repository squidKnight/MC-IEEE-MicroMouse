/*
Written by squidKnight, Mathazzar
Last modified: 06/05/20
Purpose: run the program on the simulator, which requires a main.c

NOTE: ONLY USED FOR SIMULATOR, NOT FOR USE ON ARDUINO!!
*/

#include "mouseDefs.h"
#include <stdio.h>

short int nodeList[NODES][DATA];

void nodeInit(short int nodeList[NODES][DATA]); //initialize nodeList
void scan(short int nodeList[NODES][DATA], short int position[2], short int direction); //scans the maze

int main()
{
	short int position[2] = { 0, 0 };
	short int direction = 0;

	nodeInit(nodeList);
	scan(nodeList, position, direction);
	for (int i = 0; i < NODES; i++)
	{
		fprintf(stderr, "%d;\t%d, %d, %d, %d;\t%d, %d, %d, %d\n", i, nodeList[i][NODEID], nodeList[i][NODEID_T], nodeList[i][NODEID_R], nodeList[i][NODEID_B], nodeList[i][NODEID_L], nodeList[i][DIST_T], nodeList[i][DIST_R], nodeList[i][DIST_B], nodeList[i][DIST_L]);
		fflush(stderr);
	}
	return 0;
}
