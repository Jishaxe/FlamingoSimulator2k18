#include "cAnimatedSprite.h"



// Renders at the current frame
void cAnimatedSprite::render(SDL_Renderer * theRenderer)
{
	// Get the SDL_Rect representing the current frame from the current animation
	SDL_Rect* frameRect = &this->currentAnimation->frames[this->currentFrame];
	SDL_Rect destRect = SDL_Rect();

	destRect.x = this->getSpritePos().x;
	destRect.y = this->getSpritePos().y;

	destRect.w = frameRect->w;
	destRect.h = frameRect->h;

	// Render the current frame
	this->texture->renderTexture(theRenderer, this->texture->getTexture(), frameRect, &destRect, FPoint() = { 1, 1 });
}

void cAnimatedSprite::update(double deltaTime)
{
	// If FPS is 0 then don't bother switching frames
	if (this->currentAnimation->fps == 0) return;

	// Add the deltatime to the seconds elapsed
	this->secondsSinceLastAnimationFrame += deltaTime;

	// If enough seconds have elapsed to trigger the next frame
	if (this->secondsSinceLastAnimationFrame * 1000 > 1000 / this->currentAnimation->fps) {
		this->secondsSinceLastAnimationFrame = 0; // Reset timer
		this->currentFrame++;

		// If adding this frame took it over the max frames in the animation, loop back to 0
		if (this->currentFrame == this->currentAnimation->frames.size()) this->currentFrame = 0;
	}
}

void cAnimatedSprite::addAnimation(std::string name, int frames[], int frameCount, int framesPerSecond)
{
	Animation animation;
	animation.name = name;
	animation.fps = framesPerSecond;

	// For each frame index in the array passed
	for (int i = 0; i < frameCount; i++) {
		int frameIndex = frames[i];
		SDL_Rect frameRect;

		// y will always be 0 as it is only ever on one row
		frameRect.y = 0;
		frameRect.w = this->cellWidth;

		// height will always be the height of the texture for reason above
		frameRect.h = this->texture->getTHeight();

		// The x is the frame index multiplied by the width of the cell
		frameRect.x = frameIndex * this->cellWidth;
		animation.frames.push_back(frameRect);
	}

	// Now add the animation
	this->animations.push_back(animation);

	// And set the current animation to this
	this->setAnimation(name);
}

// Set the animation by name
void cAnimatedSprite::setAnimation(std::string name)
{
	// Search for the animation and grab the reference when found
	for (int i = 0; i < this->animations.size(); i++) {
		if (this->animations[i].name == name) this->currentAnimation = &this->animations[i];
	}

	this->currentFrame = 0; // And reset the current frame
}

cAnimatedSprite::cAnimatedSprite()
{
}

cAnimatedSprite::cAnimatedSprite(cTexture* texture, int cellWidth)
{
	this->texture = texture;
	this->cellWidth = cellWidth;
	this->currentFrame = 0;
	this->secondsSinceLastAnimationFrame = 0;
}

cAnimatedSprite::~cAnimatedSprite()
{
}
