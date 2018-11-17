#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "cFontMgr.h"
#include "cTexture.h"
#include "GameConstants.h"


using namespace std;

class cScoreManager
{
public:
	cScoreManager();
	~cScoreManager();
	int currentScore;
	int highScore;
	void render(SDL_Renderer* theRenderer);
	void update(double deltaTime);
	void saveScore();
	void loadScore();
};

