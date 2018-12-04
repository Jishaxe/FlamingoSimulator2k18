#include "cObstacleManager.h"


void cObstacleManager::update(double deltaTime)
{
	SDL_Rect airObstaclePos = airObstacle.getSpritePos();
	airObstaclePos.x -= FLOOR_SPEED / 1.5;
	airObstacle.setSpritePos(airObstaclePos);
	airObstacle.update(deltaTime);

	// Move all the floor obstacles by floor speed
	for (int i = 0; i < floorObstacles.size(); i++) {
		cSprite* floor = &floorObstacles[i];
		SDL_Rect position = floor->getSpritePos();

		position.x -= FLOOR_SPEED;// *deltaTime;

		floor->setSpritePos(position);
	}

	// Move all the cloud obstacles but slower
	for (int i = 0; i < clouds.size(); i++) {
		cSprite* cloud = &clouds[i];
		SDL_Rect position = cloud->getSpritePos();

		position.x -= FLOOR_SPEED / 5;

		cloud->setSpritePos(position);
	}


	// Do we choose to spawn a floor obstacle in this update? 2% chance
	// Also don't spawn any cacti when there is a bee
	if (rand() % 100 + 1 > 99 && airObstacle.getSpritePos().x < 0) {

		// Okay, we're spawning an obstacle, but do we spawn it in a set of 1, 2 or 3?
		int setOfCacti = rand() % 100 + 1;

		// 50% chance of one obstacle
		if (setOfCacti < 50) {
			spawnFloorObstacle(WINDOW_WIDTH + 50);
		}
		// 30% of 2
		else if (setOfCacti > 50 && setOfCacti < 80) {
			spawnFloorObstacle(WINDOW_WIDTH + 50);
			spawnFloorObstacle(WINDOW_WIDTH + 200);
		}
		// 10% of 1
		else {
			spawnFloorObstacle(WINDOW_WIDTH + 50);
			spawnFloorObstacle(WINDOW_WIDTH + 200);
			spawnFloorObstacle(WINDOW_WIDTH + 350);
		}
	}

	if (rand() % 1000 + 1 > 995) spawnAirObstacle();
	if (rand() % 1000 + 1 > 990) spawnCloud();
}


// Spawn or reuse a floor obstacle
void cObstacleManager::spawnFloorObstacle(int xposition)
{
	// If we haven't reached the pool max size, spawn another obstacle
	if (floorObstacles.size() < FLOOR_OBSTACLE_POOL) {
		// Randomly pick a texture for the floor
		int texturePick = rand() % floorObstaclesTextures.size();
		cTexture* texture = floorObstaclesTextures[texturePick];

		// Create the sprite, set the texture and the position
		cSprite cactus = cSprite();
		cactus.setTexture(texture);
		cactus.setSpritePos({ xposition, WINDOW_HEIGHT - floorHeight - texture->getTHeight() });

		// Add to the floorObstacles list
		floorObstacles.push_back(cactus);
	}
	else {
		// If we have reached the max pool size, find the left most obstacle and replace it at the right
		int leftMostX = -150;
		cSprite* leftMostObstacle = NULL;

		// Keep going through all the floor obstacles, re-setting the leftMostObstacle every time we find a new obstacle
		for (int i = 0; i < floorObstacles.size(); i++) {
			cSprite* ob = &floorObstacles[i];
			SDL_Rect pos = ob->getSpritePos();
			if (pos.x < leftMostX) {
				leftMostX = pos.x;
				leftMostObstacle = ob;
			}
		}

		// As long as we found an obstacle to the left of the screen, reset its texture and move it to the front again
		if (leftMostObstacle != NULL) {
			int texturePick = rand() % floorObstaclesTextures.size();
			cTexture* texture = floorObstaclesTextures[texturePick];

			// Create the sprite, set the texture and the position
			leftMostObstacle->setTexture(texture);
			leftMostObstacle->setSpritePos({ xposition, WINDOW_HEIGHT - floorHeight - texture->getTHeight() });
		}
	}
}

// Spawn or reuse a cloud
void cObstacleManager::spawnCloud()
{
	// If we haven't reached the pool max size, spawn another obstacle
	if (clouds.size() < CLOUD_POOL) {
		// Randomly pick a texture for the floor
		int texturePick = rand() % cloudTextures.size();
		cTexture* texture = cloudTextures[texturePick];

		// Create the sprite, set the texture and the position
		cSprite cloud = cSprite();
		cloud.setTexture(texture);
		cloud.setSpritePos({ WINDOW_WIDTH + 150, CLOUD_BASE_HEIGHT + rand() % 200 - 100 });

		// Add to the floorObstacles list
		clouds.push_back(cloud);
	}
	else {
		// If we have reached the max pool size, find the left most obstacle and replace it at the right
		int leftMostX = -150;
		cSprite* leftMostCloud = NULL;

		// Keep going through all the floor obstacles, re-setting the leftMostObstacle every time we find a new obstacle
		for (int i = 0; i < clouds.size(); i++) {
			cSprite* ob = &clouds[i];
			SDL_Rect pos = ob->getSpritePos();
			if (pos.x < leftMostX) {
				leftMostX = pos.x;
				leftMostCloud = ob;
			}
		}

		// As long as we found an cloud to the left of the screen, reset its texture and move it to the front again
		if (leftMostCloud != NULL) {
			int texturePick = rand() % cloudTextures.size();
			cTexture* texture = cloudTextures[texturePick];

			// Create the sprite, set the texture and the position
			leftMostCloud->setTexture(texture);
			leftMostCloud->setSpritePos({ WINDOW_WIDTH + 150, CLOUD_BASE_HEIGHT + rand() % 200 - 100 });
		}
	}
}

void cObstacleManager::spawnAirObstacle()
{
	if (airObstacle.getSpritePos().x < -150) airObstacle.setSpritePos({ WINDOW_WIDTH + 150, AIR_OBSTACLE_Y });
}

void cObstacleManager::render(SDL_Renderer * renderer)
{
	// Render bee
	airObstacle.render(renderer);

	// Render cacti
	for (int i = 0; i < floorObstacles.size(); i++) {
		cSprite* floor = &floorObstacles[i];
		SDL_Rect destRect = floor->getSpritePos();
		destRect.w = floor->getTexture()->getTWidth();
		destRect.h = floor->getTexture()->getTHeight();
		floor->render(renderer, &floor->getSpriteDimensions(), &destRect, FPoint() = { 1, 1 });
	}

	// Render clouds
	for (int i = 0; i < clouds.size(); i++) {
		cSprite* cloud = &clouds[i];
		SDL_Rect destRect = cloud->getSpritePos();
		destRect.w = cloud->getTexture()->getTWidth();
		destRect.h = cloud->getTexture()->getTHeight();
		cloud->render(renderer, &cloud->getSpriteDimensions(), &destRect, FPoint() = { 1, 1 });
	}
}

cObstacleManager::cObstacleManager()
{
	srand(time(NULL));
}


cObstacleManager::~cObstacleManager()
{
}
