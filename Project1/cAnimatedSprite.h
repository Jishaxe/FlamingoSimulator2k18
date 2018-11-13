#pragma once
#include "cTexture.h"
#include "cSprite.h"
#include <vector>

// Contains data about an animation
struct Animation{
	std::string name;
	std::vector<SDL_Rect> frames; // Contains rectangles defining each frame
	int fps;
};

// Extension of cSprite that allows for animation with a spritesheet.
// Only supports horizontal spritesheets
class cAnimatedSprite: public cSprite
{
private:
	cTexture* texture; // The texture map containing the frames
	int cellWidth; // Width of each cell
	int currentFrame; // Current frame, is an index to the frames list of the current animation
	std::vector<Animation> animations; // List of animations. TODO: This would be better as a hashset.
	float secondsSinceLastAnimationFrame; // Times the amount of seconds elapsed since the last frame was switched
	Animation* currentAnimation;
public:
	// Renders the animation. Calculates destination rectangle from the underlying sprite's position and the source rectangle from the current frame.
	void render(SDL_Renderer* theRenderer);
	void update(double deltaTime);
	// Adds an animation to the list, takes its name and an array of frame indices relative to the beginning of the spriteshet
	void addAnimation(std::string name, int frames[], int frameCount, int framesPerSecond);

	// Sets the currently playing animation
	void setAnimation(std::string name);
	cAnimatedSprite();
	cAnimatedSprite(cTexture* texture, int cellWidth);
	~cAnimatedSprite();
};

