/*
Written by squidKnight, Mathazzar
Last modified: 04/7/20
Purpose: check to see if the current location is a node
Status: finished, tested

NOTE: ONLY COMPATABLE IN SIMULATOR!! Need to translate and incorperate finished movement/sensor functions once algorithm in stable
NOTE: most of the simulator-based functions start with "API_" so any that have this prefix need to be replaced for the physical bot
*/

#include "API.h" //only needed for simulator use
#include <stdbool.h>
#include <stdio.h>

/*int nodeCheck()
INPUTS:
RETURNS: int status
	status: 0=clear path, 1=node, -1=deadend
*/
int nodeCheck()
{
	//count number of avalible paths
	int status, paths = 3;
	if (API_wallFront()) //NOTE: the simulator cannot detect walls that are not next to it (this implementation will need to change for physical bot)
		paths--;
	if (API_wallRight())
		paths--;
	if (API_wallLeft())
		paths--;

	//determine status
	switch (paths)
	{
	case 0: //deadend
		status = -1;
		break;
	case 1: //clear path
		status = 0; //assume it will be a clear path
		break;
	case 2: //is a node
		status = 1;
		break;
	case 3: //is also a node
		status = 1;
		break;
	default:
		status = -2; //should not happen...
	}
	return status;
}
