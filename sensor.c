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

int getUS(int Trig, int Echo) //ask which ultra-sonic sensor to read
{
  //Clear Trigger Pin
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  
  //10 us pulse
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  //measure echo
  long t = pulseIn(Echo, HIGH);
  return (t * 0.017);//time * speed of sound (0.034 cm/us) / 2
}

//Placeholder, not used b/c it's easier just to check the encoder directly in the motor function(s)
int getEn(int CLK, int DT)
{
  return 0;
}
