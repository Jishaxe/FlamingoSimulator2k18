#include "cContinuousFloor.h"

void cContinuousFloor::update(double deltaTime) {
	this->x -= FLOOR_SPEED;//*deltaTime;

	// If this is the first update, fill the segment array to hit both sides of the window
	if (this->segments.size() == 0) {
		// Get the width of a segment
		segmentWidth = textures[0]->getTWidth();

		// Calculate the amount of segments needed to fill the screen
		int segmentCountToFillScreen = (int)ceil(this->screenWidth / segmentWidth);

		// Now populate the segments going from left to right (with an extra two either directionas a buffer)
		for (int i = -2; i < segmentCountToFillScreen + 2; i++) {
			FloorSegmentPosition segment;
			segment.texture = this->getRandomTexture();
			segment.rect.w = segment.texture->getTWidth();
			segment.rect.h = segment.texture->getTHeight();
			segment.rect.x = segmentWidth * i;
			segment.rect.y = screenHeight - segmentWidth; // As the segments are square images, width = height
			segments.push_back(segment);
		}
	}

	FloorSegmentPosition* rightSegment;

	// is the right side of the camera further than the rightmost segment?
	// If so, get the left segment and move that to the right so the floor repeats
	while (abs(this->x) + screenWidth > (rightSegment = this->getRightSegment())->rect.x + segmentWidth) {
		FloorSegmentPosition* leftSegment = this->getLeftSegment();
		leftSegment->rect.x = rightSegment->rect.x + segmentWidth;
	}
}


// Gets the segment with the highest x
FloorSegmentPosition* cContinuousFloor::getRightSegment() {
	FloorSegmentPosition* result = NULL;
	int highestX = 0;

	for (unsigned int i = 0; i < segments.size(); i++) {
		if (segments[i].rect.x > highestX) {
			result = &segments[i];
			highestX = result->rect.x;
		}
	}

	return result;
}

// Gets the segment with the lowest x
FloorSegmentPosition* cContinuousFloor::getLeftSegment() {
	FloorSegmentPosition* result = NULL;
	int lowestX = 9999999;

	for (unsigned int i = 0; i < segments.size(); i++) {
		if (segments[i].rect.x < lowestX) {
			result = &segments[i];
			lowestX = result->rect.x;
		}
	}

	return result;
}


// Picks a random texture out of the ones loaded
cTexture* cContinuousFloor::getRandomTexture() {
	int i = rand() % FLOOR_TEXTURE_COUNT;
	return textures[i];
}

cContinuousFloor::cContinuousFloor() {

}

void cContinuousFloor::render(SDL_Renderer* theRenderer) {
	// Render the floor segments
	for (unsigned int i = 0; i < this->segments.size(); i++) {
		// Get this segment and ts texture and rectangle
		FloorSegmentPosition segment = this->segments[i];
		SDL_Texture* tex = segment.texture->getTexture();
		SDL_Rect dest = segment.rect;

		// Add the camera position to the segment position so it moves
		dest.x += this->x;

		// Render this segment
		segment.texture->renderTexture(theRenderer, tex, NULL, &dest, FPoint() = { 1, 1 });
	}	
}

cContinuousFloor::cContinuousFloor(int screenWidth, int screenHeight)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	// Seed the random engine
	srand((unsigned int)time(NULL));
}




cContinuousFloor::~cContinuousFloor()
{
}
