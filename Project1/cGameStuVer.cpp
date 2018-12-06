/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();

/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}

using namespace std;

void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 255, 255, 255, 255);
	SDL_RenderPresent(theRenderer);

	this->m_lastTime = high_resolution_clock::now();

	theTextureMgr->setRenderer(theRenderer);
	theTextureMgr->addTexture("theBackground", "Images\\starscape1024x768.png");
	
	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	theFontMgr->initFontLib();
	vector<LPCSTR> fontList = { "unifont" };
	vector<LPCSTR> fontsToUse = { "Fonts/unifont.ttf"};
	for (int fonts = 0; fonts < (int)fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}

	// TODO: Pass width automatically
	floor = cContinuousFloor(WINDOW_WIDTH, WINDOW_HEIGHT);
	// Load textures in from 1 to FLOOR_TEXTURE_COUNT. Uses the number as a prefix
	for (int i = 1; i < FLOOR_TEXTURE_COUNT + 1; i++) {
		stringstream fileNameStream;
		fileNameStream << "Images\\floor" << i << ".png";
		std::string fileNameString = fileNameStream.str();

		// Load the texture
		theTextureMgr->addTexture(fileNameString.c_str(), fileNameString.c_str());

		// Now add the pointer to this texture in the floor
		floor.textures[i - 1] = theTextureMgr->getTexture(fileNameString.c_str());

		floorHeight = theTextureMgr->getTexture(fileNameString.c_str())->getTHeight();
	}

	// Now load the main player, passing the player spritesheet
	theTextureMgr->addTexture("player", "Images\\player.png");
	playerSprite = cAnimatedSprite(theTextureMgr->getTexture("player"), 150); // Each cell width is 150

	// Define the animations and the frame indices for them in the spritesheet
	int runFrames[] = { 0, 1, 2 };
	int jumpFrames[] = { 3 };
	int duckFrames[] = { 4 };


	// Now add the three animations 
	playerSprite.addAnimation("run", runFrames, 3, 15);
	playerSprite.addAnimation("jump", jumpFrames , 1, 0);
	playerSprite.addAnimation("duck", duckFrames, 1, 0);
	playerSprite.setAnimation("run");

	// Set the player position to sit on the floor
	playerSprite.setSpritePos({ PLAYER_X, WINDOW_HEIGHT - floorHeight - theTextureMgr->getTexture("player")->getTHeight() }); // Screen height take away the floor height take away player height

	// Give the sprite to the playercontroller so it can control the player
	playerController.playerSprite = &playerSprite;
	playerController.lowestY = WINDOW_HEIGHT - floorHeight - theTextureMgr->getTexture("player")->getTHeight();

	// Give the two cactus textures to the obstacle manager
	obstacleManager.floorObstaclesTextures.push_back(theTextureMgr->addTexture("cactus1", "Images\\cactus1.png"));
	obstacleManager.floorObstaclesTextures.push_back(theTextureMgr->addTexture("cactus2", "Images\\cactus2.png"));

	// And the three cloud textures
	obstacleManager.cloudTextures.push_back(theTextureMgr->addTexture("cloud1", "Images\\cloud1.png"));
	obstacleManager.cloudTextures.push_back(theTextureMgr->addTexture("cloud2", "Images\\cloud2.png"));
	obstacleManager.cloudTextures.push_back(theTextureMgr->addTexture("cloud3", "Images\\cloud3.png"));

	// And add the bee animation
	theTextureMgr->addTexture("bee", "Images\\bee.png");
	obstacleManager.airObstacle = cAnimatedSprite(theTextureMgr->getTexture("bee"), 150);
	int flyFrames[] = { 0, 1 };
	obstacleManager.airObstacle.addAnimation("fly", flyFrames, 2, 3);

	// Just hide the air obstacle for now
	obstacleManager.airObstacle.setSpritePos({ -150, AIR_OBSTACLE_Y });

	obstacleManager.floorHeight = floorHeight;

	scoreManager.loadScore();

	// And the main menu
	mainMenu.background.setTexture(theTextureMgr->addTexture("mainmenu", "Images\\mainmenu.png"));
	mainMenu.gameOverBackground.setTexture(theTextureMgr->addTexture("endmenu", "Images\\endmenu.png"));

	mainMenu.goButtonUnpressed = theTextureMgr->addTexture("go", "Images\\go.png");
	mainMenu.goButtonPressed = theTextureMgr->addTexture("gopressed", "Images\\gopressed.png");
	mainMenu.quitButtonUnpressed = theTextureMgr->addTexture("quit", "Images\\quit.png");
	mainMenu.quitButtonPressed = theTextureMgr->addTexture("quitPressed", "Images\\quitPressed.png");
	mainMenu.replayButtonUnpressed = theTextureMgr->addTexture("replayButton", "Images\\replayButton.png");
	mainMenu.replayButtonPressed = theTextureMgr->addTexture("replayButtonPressed", "Images\\replayButtonPressed.png");
}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

	// Render the floor segments
	for (unsigned int i = 0; i < this->floor.segments.size(); i++) {
		// Get this segment and its texture and rectangle
		FloorSegmentPosition segment = this->floor.segments[i];
		SDL_Texture* tex = segment.texture->getTexture();
		SDL_Rect dest = segment.rect;

		// Add the camera position to the segment position so it moves
		dest.x += this->floor.x;

		// Render this segment
		segment.texture->renderTexture(theRenderer, tex, NULL, &dest, FPoint() = { 1, 1 });
	}

	obstacleManager.render(theRenderer);
	playerSprite.render(theRenderer);

	//SDL_RenderDrawRect(theRenderer, &playerBoundingBox);

	if (mainMenu.active) {
		mainMenu.render(theRenderer);
	}

	if (gameover) {
		mainMenu.renderGameOver(theRenderer);
	}

	scoreManager.render(theRenderer);

	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{
	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{
}

void cGame::update(double deltaTime)
{
	if (!gameover && !mainMenu.active) {
		// If the game is not over and the main menu is not active, update all the components of the main game
		floor.update(deltaTime);
		playerSprite.update(deltaTime);
		playerController.update(deltaTime);
		obstacleManager.update(deltaTime);
		scoreManager.update(deltaTime);
		
		// Increase the score for every tick we are alive
		scoreManager.currentScore++;

		// And increase the high score if the current score is higher
		if (scoreManager.currentScore > scoreManager.highScore) scoreManager.highScore = scoreManager.currentScore;

		// The player's bounding box is smaller than the player
		playerBoundingBox = { playerSprite.getSpritePos().x + 25, playerSprite.getSpritePos().y + 25, 100, 100 };

		// And when it is ducking the box goes down lower
		if (playerController.isDucking) playerBoundingBox.y += 50;

		// See if it's colliding with any of the floor obstacles
		for (unsigned int i = 0; i < obstacleManager.floorObstacles.size(); i++) {
			cSprite* obstacle = &obstacleManager.floorObstacles[i];
			if (obstacle->isCollidingWith(&playerBoundingBox)) {
				gameover = true;

				playerController.verticalVelocity = -JUMP_POWER;

				// Save the high score
				scoreManager.saveScore();
				continue;
			}
		}

		// See if it's colliding with the air obstacle
		if (obstacleManager.airObstacle.isCollidingWith(&playerBoundingBox)) {
			gameover = true;

			// Save the high score
			scoreManager.saveScore();
		}
	}

	// Only update the main menu if it's active
	if (mainMenu.active) {
		mainMenu.update(deltaTime);
	}

	if (gameover) {
		playerController.deathAnimation(deltaTime);
		mainMenu.updateGameOver(deltaTime);
	}
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_w:
				case SDLK_SPACE:
				case SDLK_UP:
					// Make the player jump here
					playerController.isJumpHeldDown = true;
					break;
				case SDLK_s:
				case SDLK_LCTRL:
				case SDLK_RCTRL:
				case SDLK_DOWN:
					playerController.isDuckHeldDown = true;
					// Make the player duck here
					break;
				default:
					break;
			}
		}

		if (event.type == SDL_KEYUP) {
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_w:
				case SDLK_SPACE:
				case SDLK_UP:
					// Make the player not jump here
					playerController.isJumpHeldDown = false;
					break;
				case SDLK_s:
				case SDLK_LCTRL:
				case SDLK_RCTRL:
				case SDLK_DOWN:
					// Make the player not sduck here
					playerController.isDuckHeldDown = false;
					break;
				default:
					break;
			}
		}
	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Save the high score
	scoreManager.saveScore();

	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

void cGame::replay()
{
	scoreManager.currentScore = 0;
	playerSprite.setAnimation("run");
	playerController.isOnFloor = true;
	playerController.isDucking = false;

	// Move all the obstacles to the left of the screen so they can respawn
	for (unsigned int i = 0; i < obstacleManager.floorObstacles.size(); i++) {
		SDL_Rect currentPos = obstacleManager.floorObstacles[i].getSpritePos();
		currentPos.x = -200;
		obstacleManager.floorObstacles[i].setSpritePos(currentPos);
	}


	// And the bee too
	SDL_Rect beePos = obstacleManager.airObstacle.getSpritePos();
	beePos.x = -200;
	obstacleManager.airObstacle.setSpritePos(beePos);
}

