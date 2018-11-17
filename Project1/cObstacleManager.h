#pragma once
#include <vector>
#include <sstream>
#include "cSprite.h"
#include "cAnimatedSprite.h"

class cObstacleManager
{
	public:
		int floorHeight;
		// I want to maintain a "pool" of obstacles, aka: first cactus on left is moved back to the right.
		// Similar to how the floors work
		std::vector<cSprite> floorObstacles;
		std::vector<cSprite> clouds;
		cAnimatedSprite airObstacle;

		std::vector<cTexture*> floorObstaclesTextures;
		std::vector<cTexture*> cloudTextures;

		void update(double deltaTime);
		void spawnFloorObstacle(int xposition); // Spawns a set of floor obstacles from ones that have reached the left of the screen
		void spawnCloud(); // Spawns a set of floor obstacles from ones that have reached the left of the screen
		void spawnAirObstacle(); // Spawns a set of air obstacles from ones that have reached the left of the screen
		void render(SDL_Renderer* renderer);

		cObstacleManager();
		~cObstacleManager();
};

