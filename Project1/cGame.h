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
#include "flamingoRun.h"
#include "cObstacleManager.h"
#include "cPlayerController.h"
#include "cContinuousFloor.h"
#include "cAnimatedSprite.h"
#include "cScoreManager.h"
#include "cSoundMgr.h"
#include "cInput.h"
#include "cFontMgr.h"
#include "cMenuManager.h"


using namespace std;

class cGame
{
public:
	cGame();

	void initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void cleanUp(SDL_Window* theSDLWND);
	void replay();
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre);
	void update();
	void update(double deltaTime);
	double getElapsedSeconds();
	static cGame* getInstance();
	gameState theGameState;
	JumpOrDuckInputState inputState;
	bool loop = true;
	int floorHeight;
private:
	cMenuManager menuManager;
	cInput input;
	static cGame* pInstance;
	// for framerates
	time_point< high_resolution_clock > m_lastTime;
	time_point< high_resolution_clock > m_CurrentTime;
	duration< double > deltaTime;

	// Game Sounds
	vector<LPCSTR> soundList;
	vector<soundType> soundTypes;
	vector<LPCSTR> soundsToUse;

	// game related variables
	// Sprites for displaying background and rocket textures
	cSprite spriteBkgd;
	cContinuousFloor floor;
	cAnimatedSprite playerSprite;
	cPlayerController playerController;
	cObstacleManager obstacleManager;
	cScoreManager scoreManager;
	SDL_Rect playerBoundingBox;
	// Game objects
};

#endif
