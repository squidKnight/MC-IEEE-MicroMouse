/*
Written by squidKnight
Last modified: 03/15/20
Purpose: run the program on the simulator, which requires a main.c

NOTE: ONLY USED FOR SIMULATOR, NOT FOR USE ON ARDUINO!!
*/

void nodeInit(); //initialize nodeList
void scan(); //scans the maze

int main()
{
	nodeInit();
	scan();
	return 0;
}
