/*
=================
cRocket.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _CROCKET_H
#define _CROCKET_H
#include "cSprite.h"

class cRocket : public cSprite
{
private:
	int rocketVelocity;
	double rocketRotAngle;
	double rocketDrag;

public:
	bool rotatingLeft;
	bool rotatingRight;
	bool goingForward;
	bool goingBackwards;

	void initialise();
	//void render();		// Default render function
	void update(double deltaTime);		// Rocket update method
	void setRocketVelocity(int rocketVel);   // Sets the velocity for the rocket
	int getRocketVelocity();				 // Gets the rocket velocity
	void setRocketRotation(double theRotAngle);   // Sets the rotation angle for the rocket
	double getRocketRotation();				 // Gets the rotation angle for the rocket velocity
	void addRotation(double toAdd); // Adds to the rotation of the rocket, wrapping around at 360
	
};
#endif