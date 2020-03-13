/*
Written by squidKnight
Last modified: 03/13/20
Purpose: allow for easy control of motors by creating movement functions
Status: UNFINISHED, NOT TESTED
*/

#include "portDefs.h"

#define RADIUS 1 //radius of wheel in cm
#define WALL 18 //length of one wall segment in cm
#define RESUOLTION 12 //number of counts in one full rotation of the quadrature encoder
#define PI 3.1415 //close enough

int forward(short int dist, short int speed) //move forward a specific distance and speed (pwm)
{
	short int positionL = 0; //counter for the left quad's position, asssigns current position as 0
	short int positionR = 0; //same as above, but for right quad
	float target = dist * ( WALL/(2 * PI * RADIUS) ); //number of rotations needed to go dist wall lengths (dist x ratio of wall length to wheel ciscumferance)

	//quick error checking
	if(speed > 250)
		speed = 250; //255 is pwm max, but 250 is used to allow program to compensate for possible motor drift

	//check to see if at taget distance
	while(positionL < target || positionR < target) //while not yet at target distance
	{
		//compensate for speed differences, if needed
		if(positionL > positionR) //if left is going faster than right
		{
			analogWrite(MOTOR_LEFT, speed);
			analogWrite(MOTOR_RIGHT, speed + 5); //slower motor receieves a 2 percent speed boost
		}
		else if(positionL < positionR) //if right is going faster than left
		{
			analogWrite(MOTOR_LEFT, speed + 5);
			analogWrite(MOTOR_RIGHT, speed);
		}
		else //if motors are going at the same rate, assign default speeds
		{
			analogWrite(MOTOR_LEFT, speed);
			analogWrite(MOTOR_RIGHT, speed);
		}

		//check to see if quads are at a count slot
		if(digitalRead(QUAD_LEFT_CLK) == 1) //if left quad CLK output is at a slot
			positionL++; //CW or CCW rotation does not matter (speed is always in the same direction), so only need to consider increment case 
		if(digitalRead(QUAD_RIGHT_CLK) == 1)
			positionR++;
	}
	
	//done, turn off motors
	analogWrite(MOTOR_LEFT, 0);
	analogWrite(MOTOR_RIGHT, 0);
	
	//result output
	Serial.print("Final left motor position: ");
	Serial.print(positionL);
	Serial.print("Final right motor position: ");
	Serial.print(positionR);
	Serial.print("Approximate percent error from target distance: ");
	Serial.print( ((positionL + positionR)/24.0) / target); //divided by 24 since average means divide by 2 and total rotations means divide by 12
}
