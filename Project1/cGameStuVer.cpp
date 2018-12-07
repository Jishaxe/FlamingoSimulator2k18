/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

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
	theGameState = gameState::mainmenu;
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 255, 255, 255, 255);
	SDL_RenderPresent(theRenderer);

	this->m_lastTime = high_resolution_clock::now();

	theTextureMgr->setRenderer(theRenderer);
	theTextureMgr->addTexture("theBackground", "Images\\background.png");
	
	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	vector<LPCSTR> fontList = { "unifont" };
	vector<LPCSTR> fontsToUse = { "Fonts/unifont.ttf"};
	for (int fonts = 0; fonts < (int)fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}

	// Load game sounds
	soundList = { "mainmenutheme", "ingametheme", "endgametheme", "jump", "death", "duck"};
	soundTypes = { soundType::music, soundType::music, soundType::music, soundType::sfx, soundType::sfx, soundType::sfx};
	soundsToUse = { "Sounds/mainmenutheme.mp3", "Sounds/ingametheme.mp3", "Sounds/endgametheme.mp3", "Sounds/jump.wav", "Sounds/death.wav", "Sounds/duck.mp3"};
	for (unsigned int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	// Play the main menu theme
	theSoundMgr->getSnd("mainmenutheme")->play(-1);

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
	menuManager.background.setTexture(theTextureMgr->addTexture("mainmenu", "Images\\mainmenu.png"));
	menuManager.gameOverBackground.setTexture(theTextureMgr->addTexture("endmenu", "Images\\endmenu.png"));

	menuManager.goButtonUnpressed = theTextureMgr->addTexture("go", "Images\\go.png");
	menuManager.goButtonPressed = theTextureMgr->addTexture("gopressed", "Images\\gopressed.png");
	menuManager.quitButtonUnpressed = theTextureMgr->addTexture("quit", "Images\\quit.png");
	menuManager.quitButtonPressed = theTextureMgr->addTexture("quitPressed", "Images\\quitPressed.png");
	menuManager.replayButtonUnpressed = theTextureMgr->addTexture("replayButton", "Images\\replayButton.png");
	menuManager.replayButtonPressed = theTextureMgr->addTexture("replayButtonPressed", "Images\\replayButtonPressed.png");

	menuManager.goButton.setTexture(menuManager.goButtonUnpressed);
	menuManager.quitButton.setTexture(menuManager.quitButtonUnpressed);
	menuManager.replayButton.setTexture(menuManager.replayButtonUnpressed);
}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		inputState = input.getInput();
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

	// Render main menu if that is the state
	if (theGameState == gameState::mainmenu) {
		menuManager.render(theRenderer);
	}

	// Render game over if that is the state
	if (theGameState == gameState::gameover) {
		menuManager.renderGameOver(theRenderer);
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
	switch (theGameState) {
		case gameState::ingame:
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
					std::cout << "Bounding boxes have touched, doing pixel perfect checks" << std::endl;

					// Bounding boxes are colliding, now do pixel perfect checks
					if (playerSprite.isCollidingWith(obstacle)) {
						theGameState = gameState::gameover;

						// Play the game over sound
						cSoundMgr::getInstance()->stopMusic();
						cSoundMgr::getInstance()->getSnd("endgametheme")->play(-1);

						cSoundMgr::getInstance()->getSnd("death")->play(0);
						// Launch the boye into the air
						playerController.verticalVelocity = -JUMP_POWER;

						// Save the high score
						scoreManager.saveScore();
						continue;
					}

				}
			}

			/*
			// See if it's colliding with the air obstacle
			if (obstacleManager.airObstacle.isCollidingWith(&playerBoundingBox)) {
				theGameState = gameState::gameover;

				// Play the game over sound
				cSoundMgr::getInstance()->stopMusic();
				cSoundMgr::getInstance()->getSnd("endgametheme")->play(-1);
				cSoundMgr::getInstance()->getSnd("death")->play(0);

				// Save the high score
				scoreManager.saveScore();
			}
			*/
			break;

		case gameState::mainmenu:
			menuManager.update(deltaTime);
			break;
		case gameState::gameover:
			playerController.deathAnimation(deltaTime);
			menuManager.updateGameOver(deltaTime);
			break;
	}
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
	theSoundMgr->stopMusic();
	theSoundMgr->getSnd("ingametheme")->play(-1);
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

