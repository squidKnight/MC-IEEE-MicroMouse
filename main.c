/*
Written by squidKnight, Mathazzar
Last modified: 04/24/20
Purpose: run the program on the simulator, which requires a main.c

NOTE: ONLY USED FOR SIMULATOR, NOT FOR USE ON ARDUINO!!
*/

#include "mouseDefs.h"

int nodeList[NODES][DATA];

void nodeInit(int nodeList[NODES][DATA]); //initialize nodeList
void scan(int nodeList[NODES][DATA]); //scans the maze

int main()
{
	nodeInit(nodeList);
	scan(nodeList);
	return 0;
}
