/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();

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

	theTextureMgr->addTexture("theRocket", "Images\\rocketSprite.png");
	rocketSprite.setSpritePos({ 500, 350 });
	rocketSprite.setTexture(theTextureMgr->getTexture("theRocket"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theRocket")->getTWidth(), theTextureMgr->getTexture("theRocket")->getTHeight());

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
}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	bool loop = true;

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

	playerSprite.render(theRenderer);


	// Render the floor segments
	for (int i = 0; i < this->floor.segments.size(); i++) {
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
	SDL_RenderDrawRect(theRenderer, &playerBoundingBox);
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
	if (!gameover) {
		floor.update(deltaTime);
		playerSprite.update(deltaTime);
		playerController.update(deltaTime);
		obstacleManager.update(deltaTime);
		rocketSprite.update(deltaTime);
	}

	// The player's bounding box is smaller than the player
	playerBoundingBox = { playerSprite.getSpritePos().x + 25, playerSprite.getSpritePos().y + 25, 100, 100 };

	// And when it is ducking the box goes down lower
	if (playerController.isDucking) playerBoundingBox.y += 50;

	// See if it's colliding with any of the floor obstacles
	for (int i = 0; i < obstacleManager.floorObstacles.size(); i++) {
		cSprite* obstacle = &obstacleManager.floorObstacles[i];
		if (obstacle->isCollidingWith(&playerBoundingBox)) {
			gameover = true;
			continue;
		}
	}

	// See if it's colliding with the air obstacle
	if (obstacleManager.airObstacle.isCollidingWith(&playerBoundingBox)) gameover = true;
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
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

