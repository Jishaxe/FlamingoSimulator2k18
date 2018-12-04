#pragma once
#include "cSprite.h"
class cMainMenu
{
public:
	bool active = true;
	cSprite background;
	cSprite goButton;
	cSprite quitButton;

	cTexture* goButtonUnpressed;
	cTexture* goButtonPressed;

	cTexture* quitButtonPressed;
	cTexture* quitButtonUnpressed;

	void update(double deltaTime);
	void render(SDL_Renderer* theRenderer);
	cMainMenu();
	~cMainMenu();
};

