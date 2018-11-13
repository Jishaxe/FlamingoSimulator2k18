/*
=================
cRocket.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cRocket.h"
#include <math.h>

/*
=================================================================
Initialise the sprite variables
=================================================================
*/

void cRocket::initialise()
{
	rocketVelocity = 0;
	rocketRotAngle = 0;
	rocketDrag = 0.85;
}

//void cRocket::render()
//{
//}
/*
=================================================================
Update the sprite position
=================================================================
*/

double clampRotation(double rotation) {
	if (rotation > 360) rotation = rotation - 360;
	if (rotation < 0) rotation = 360 + rotation;

	return rotation;
}

void cRocket::update(double deltaTime)
{
	SDL_Rect position = this->getSpritePos();

	if (rotatingLeft) {
		this->addRotation(-200 * deltaTime);
	}

	if (rotatingRight) {
		this->addRotation(200 * deltaTime);
	}

	if (goingForward) {

		rocketVelocity += 100 * deltaTime;
		if (rocketVelocity > 10) rocketVelocity = 10; // Clip to a max speed


	} else if (goingBackwards) {


		rocketVelocity -= 100 * deltaTime;
		if (rocketVelocity < -10) rocketVelocity = -10; // Clip to a max speed


	}
	else {
		// Slow down the rocket when not boosting
		rocketVelocity *= rocketDrag;
	}

	// Move along the forward axis (90 degrees if the rocket image is facing up)
	position.x -= this->rocketVelocity * cos(clampRotation(this->rocketRotAngle + 90) * PI / 180);
	position.y -= this->rocketVelocity * sin(clampRotation(this->rocketRotAngle + 90) * PI / 180);
	this->setSpritePos(position);
}



// Adds and wraps around the rotation
void cRocket::addRotation(double toAdd) {
	double currentRotation = this->rocketRotAngle;

	currentRotation += toAdd;

	currentRotation = clampRotation(currentRotation);

	this->rocketRotAngle = currentRotation;
}

/*
=================================================================
  Sets the velocity for the rocket
=================================================================
*/
void cRocket::setRocketVelocity(int rocketVel)
{
	rocketVelocity = rocketVel;
}
/*
=================================================================
  Gets the rocket velocity
=================================================================
*/
int cRocket::getRocketVelocity()
{
	return rocketVelocity;
}
/*
=================================================================
Sets the rotation angle for the rocket
=================================================================
*/
void cRocket::setRocketRotation(double theRotAngle)
{
	rocketRotAngle = theRotAngle;
}
/*
=================================================================
Gets the rotation angle for the rocket
=================================================================
*/
double cRocket::getRocketRotation()
{
	return rocketRotAngle;
}
