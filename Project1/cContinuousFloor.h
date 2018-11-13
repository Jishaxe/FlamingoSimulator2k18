#pragma once
#include "./cTexture.h"
#include <stdlib.h>
#include <time.h>

struct FloorSegmentPosition {
	cTexture* texture;
	SDL_Rect rect;
};

class cContinuousFloor
{
private:
	int screenWidth;
	int screenHeight;

	FloorSegmentPosition* getRightSegment();
	FloorSegmentPosition* getLeftSegment();
public:
	cTexture* textures[FLOOR_TEXTURE_COUNT];

	int x = 0;

	int segmentWidth;
	std::vector<FloorSegmentPosition> segments;
	void update(double deltaTime);
	cTexture* getRandomTexture();
	cContinuousFloor(int screenWidth, int screenHeight);
	cContinuousFloor();
	~cContinuousFloor();
};

