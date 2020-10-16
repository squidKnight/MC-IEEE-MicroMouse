/*
Written by Mathazzar
Last modified: 10/15/20
Purpose: list functions for finding adjacent nodes.
Status: FINISHED, TESTED
Notes: functions currently declared in nodeCheck.c
*/

bool nodeCheck(bool nodeCurrent[DATA]); //checks to see if the current location is a node
short int node(short int nodeCurrent); //dummy function, just returns nodeCurrent
short int node_T(short int nodeCurrent); //returns node above nodeCurrent
short int node_R(short int nodeCurrent); //returns node right of nodeCurrent
short int node_B(short int nodeCurrent); //returns node below nodeCurrent
short int node_L(short int nodeCurrent); //returns node left of nodeCurrent
