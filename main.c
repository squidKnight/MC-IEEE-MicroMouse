/*
Written by squidKnight, Mathazzar
Last modified: 04/7/20
Purpose: run the program on the simulator, which requires a main.c

NOTE: ONLY USED FOR SIMULATOR, NOT FOR USE ON ARDUINO!!
*/

#include "mouseDefs.h"

int nodeList[256][DATA][3];

void nodeInit(int nodeList[256][DATA][3]); //initialize nodeList
void scan(int nodeList[256][DATA][3]); //scans the maze

int main()
{
	nodeInit(nodeList);
	scan(nodeList);
	return 0;
}
