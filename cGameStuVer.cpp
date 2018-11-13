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
	floor = cContinuousFloor(1024, 768);
	int floorHeight;
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
	flamingo = cAnimatedSprite(theTextureMgr->getTexture("player"), 150); // Each cell width is 150

	// Define the animations and the frame indices for them in the spritesheet
	int runFrames[] = { 0, 1, 2 };
	int jumpFrames[] = { 3 };
	int duckFrames[] = { 4 };


	// Now add the three animations 
	flamingo.addAnimation("run", runFrames, 3, 15);
	flamingo.addAnimation("jump", jumpFrames , 1, 0);
	flamingo.addAnimation("duck", duckFrames, 1, 0);
	flamingo.setAnimation("run");

	flamingo.setSpritePos({ 100, 768 - floorHeight - theTextureMgr->getTexture("player")->getTHeight() }); // Screen height take away the floor height take away player height
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

	flamingo.render(theRenderer);

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
	//rocketSprite.render(theRenderer, &rocketSprite.getSpriteDimensions(), &rocketSprite.getSpritePos(), rocketSprite.getRocketRotation(), &rocketSprite.getSpriteCentre(), rocketSprite.getSpriteScale());
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
	floor.update(deltaTime);
	flamingo.update(deltaTime);
	rocketSprite.update(deltaTime);
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
				case SDLK_SPACE:
				case SDLK_UP:
					// Make the player jump here
					break;
				case SDLK_d:
				case SDLK_DOWN:
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
				case SDLK_SPACE:
				case SDLK_UP:
					// Make the player not jump here
					break;
				case SDLK_d:
				case SDLK_DOWN:
					// Make the player not sduck here
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

