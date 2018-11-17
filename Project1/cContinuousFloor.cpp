#include "cContinuousFloor.h"

void cContinuousFloor::update(double deltaTime) {
	this->x -= FLOOR_SPEED;//*deltaTime;

	// If this is the first update, fill the segment array to hit both sides of the window
	if (this->segments.size() == 0) {
		// Get the width of a segment
		segmentWidth = textures[0]->getTWidth();

		// Calculate the amount of segments needed to fill the screen
		int segmentCountToFillScreen = ceil(this->screenWidth / segmentWidth);

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

	for (int i = 0; i < segments.size(); i++) {
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
	int lowestX = 9999999999;

	for (int i = 0; i < segments.size(); i++) {
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


cContinuousFloor::cContinuousFloor(int screenWidth, int screenHeight)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	// Seed the random engine
	srand(time(NULL));
}




cContinuousFloor::~cContinuousFloor()
{
}
