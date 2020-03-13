/*
Written by Mathazzar
Last modified: 03/13/20
Purpose: allow for easy control of sensors
Status: UNFINISHED, NOT TESTED
*/

//Not needed, because none are directly called here, they're defined and called from whatever is calling these fuctions
//#include "portDefs.h"
short int IRV = 27;//27V*cm constant for reading IR sensor

int getIR(int sensor) //ask which infra-red sensor to read
{
  //constant is using best-fit line, needs to be adjusted based on testing or apply actuall graph from datasheet of GP2Y0A21YK0F
  return (IRV / analogRead(sensor));// IRV/sensor output
}

int getUS(int sensor) //ask which ultra-sonic sensor to read
{
  return 0;//PLACEHOLDER; need to know if it's an analog or digital sensor, provide the model so that its datasheet can be found
}

//Placeholder, not used b/c it's easier just to check the encoder directly in the motor function(s)
int getEn(int CLK, int DT)
{
  return 0;
}
