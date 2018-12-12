#include "cPlayerController.h"
#include "GameConstants.h"
#include "cGame.h"

cPlayerController::cPlayerController()
{
}


cPlayerController::~cPlayerController()
{
}

// Simply sinks the player through the floor
void cPlayerController::deathAnimation(double deltaTime) {
	playerSprite->setAnimation("jump");
	verticalVelocity += GRAVITY / 2;
	SDL_Rect playerPosition = playerSprite->getSpritePos();
	playerPosition.y += (int)verticalVelocity;
	playerSprite->setSpritePos(playerPosition);
}

void cPlayerController::update(double deltaTime)
{
	isJumpHeldDown = cGame::getInstance()->inputState.jumpHeldDown;
	isDuckHeldDown = cGame::getInstance()->inputState.duckHeldDown;

	// If the jump key has been held down and it is on the floor
	if (isJumpHeldDown && isOnFloor) {
		cSoundMgr::getInstance()->getSnd("jump")->play(0);
		verticalVelocity = -JUMP_POWER;
		isOnFloor = false;
		playerSprite->setAnimation("jump");
	}

	// If the duck button is held down and player is on floor, set ducking animation
	if (isDuckHeldDown && isOnFloor && !isDucking) {
		cSoundMgr::getInstance()->getSnd("duck")->play(0);
		playerSprite->setAnimation("duck");
		isDucking = true;
	}

	// If duck button is not being held down but is currently ducking, as well as the jump button isn't being held down and is also on the floor
	// then play running animation
	if (!isDuckHeldDown && isDucking && !isJumpHeldDown && isOnFloor) {
		playerSprite->setAnimation("run");
		isDucking = false;
	}

	SDL_Rect playerPosition = playerSprite->getSpritePos();
	playerPosition.y += (int)verticalVelocity;

	// If the player has sunk below the floor
	if (playerPosition.y > lowestY) {
		// Set velocity to 0, put the player back on the floor and play running animation
		verticalVelocity = 0;
		playerPosition.y = lowestY;
		if (!isOnFloor) playerSprite->setAnimation("run");
		isOnFloor = true;
		
	}
	else {
		// Apply gravity if we're in the air
		if (isJumpHeldDown) verticalVelocity += GRAVITY / 2;
		else verticalVelocity += GRAVITY;
	}

	playerSprite->setSpritePos(playerPosition);
}
