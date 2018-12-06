#pragma once
#include "cSprite.h"
class cMainMenu
{
public:
	bool active = true;
	cSprite background;
	cSprite gameOverBackground;
	cSprite goButton;
	cSprite quitButton;
	cSprite replayButton;

	cTexture* goButtonUnpressed;
	cTexture* goButtonPressed;

	cTexture* quitButtonPressed;
	cTexture* quitButtonUnpressed;

	cTexture* replayButtonPressed;
	cTexture* replayButtonUnpressed;
	void update(double deltaTime);
	void updateGameOver(double deltaTime);
	void render(SDL_Renderer* theRenderer);
	void renderGameOver(SDL_Renderer* theRenderer);
	cMainMenu();
	~cMainMenu();

private:
	double timeSinceDroppedToMainMenu; // This is set as a workaround - the exit button on the main menu will wait a 
	// moment before getting clicks after dropping from the end menu and this variable measures that time
};

