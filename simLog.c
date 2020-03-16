/*
Last modified: 03/16/20
Purpose: hold all of the alorithm-related maze functions (scaning, solving, optimizing, etc.)

NOTE: simulator-exclusive function
*/

#include <stdio.h>

void simLog(char* text) //modified from main.c in mms example (https://github.com/mackorone/mms-c)
{
	fprintf(stderr, "%s\n", text);
	fflush(stderr);
}
