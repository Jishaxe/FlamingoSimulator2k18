#ifndef _GAME_H
#define _GAME_H

/*
==================================================================================
cGame.h
==================================================================================
*/

#include <SDL.h>
#include <iostream>
#include <string>
#include <sstream>

// Game specific includes
#include "rocketGame.h"
#include "cObstacleManager.h"
#include "cContinuousFloor.h"
#include "cAnimatedSprite.h"
#include "cScoreManager.h"
#include "cFontMgr.h"


using namespace std;

class cGame
{
public:
	cGame();

	void initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void cleanUp(SDL_Window* theSDLWND);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre);
	void update();
	void update(double deltaTime);
	bool getInput(bool theLoop);
	double getElapsedSeconds();

	static cGame* getInstance();

	bool gameover = false;
	int floorHeight;
private:

	static cGame* pInstance;
	// for framerates
	time_point< high_resolution_clock > m_lastTime;
	time_point< high_resolution_clock > m_CurrentTime;
	duration< double > deltaTime;

	// game related variables
	// Sprites for displaying background and rocket textures
	cSprite spriteBkgd;
	cRocket rocketSprite;
	cContinuousFloor floor;
	cAnimatedSprite playerSprite;
	cPlayerController playerController;
	cObstacleManager obstacleManager;
	cScoreManager scoreManager;
	SDL_Rect playerBoundingBox;
	// Game objects
};

#endif
