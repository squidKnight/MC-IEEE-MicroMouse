/*
Written by Mathazzar
Last modified: 03/13/20
Purpose: allow for easy control of sensors
Status: UNFINISHED, NOT TESTED
*/

#include "portDefs.h"

int getIR(int sensor) //ask which infra-red sensor to read
{
  //constant is using best-fit line, needs to be adjusted based on testing or apply actuall graph from datasheet of GP2Y0A21YK0F
  return (27/analogRead(sensor));\\27V*cm constant/output
}

int getUS(int sensor) //ask which ultra-sonic sensor to read
{
  return 0;//PLACEHOLDER
}
