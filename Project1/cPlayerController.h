#pragma once
#include "cAnimatedSprite.h"

class cPlayerController
{
public:
	cAnimatedSprite* playerSprite;
	
	// Whether the jump or duck key is held down
	bool isJumpHeldDown = false;
	bool isDuckHeldDown = false;

	// If the player is on the floor
	bool isOnFloor = true;

	// If the player is ducking
	bool isDucking = false;

	// Vertical velocity of the player
	double verticalVelocity = 0;

	// the lowest Y that the player can be (the floor)
	int lowestY;
	void update(double deltaTime);
	void deathAnimation(double deltaTime);
	cPlayerController();
	~cPlayerController();
};

