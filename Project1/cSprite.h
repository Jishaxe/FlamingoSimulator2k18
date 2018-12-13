/*
=================
cSprite.h
- Header file for class definition - SPECIFICATION
- Header file for the Sprite class
=================
*/
#ifndef _CSPRITE_H
#define _CSPRITE_H

#include <cmath>
#include "cTextureMgr.h"
#include "cTexture.h"

class cSprite
{
private:
	// position
	SDL_Rect spritePos_2D;
	SDL_Rect spriteDimensions;
	SDL_Point spriteCentre;
	FPoint spriteScale;

	// Pointer to Sprite Texture
	cTexture* spriteTexture;

	// Texture width & Height
	int textureWidth;
	int textureHeight;

	SDL_Rect intersectRect;
public:
	cSprite();			// Default constructor
	cSprite(cTexture* theSpriteTexture);			// Default constructor
	~cSprite();			// Destructor
	void render(SDL_Renderer* theRenderer, SDL_Rect* theSourceRect, SDL_Rect* theDestRect, FPoint theScaling);
	void render(SDL_Renderer* theRenderer, SDL_Rect* theSourceRect, SDL_Rect* theDestRect, double rotAngle, SDL_Point* spriteCentre, FPoint theScaling);
	SDL_Rect getSpritePos();  // Return the sprites current position
	void setSpritePos(SDL_Rect sPosition); // set the position of the sprite
	cTexture* getTexture();  // Return the sprites current image
	void setTexture(cTexture* theSpriteTexture);  // set the image of the sprite
	void setSpriteDimensions(int texWidth, int textHeight);
	SDL_Rect getSpriteDimensions(); // Return sprite dimensions
	SDL_Point getSpriteCentre();  // Return the sprites current position
	void setSpriteCentre(SDL_Point sCentre); // set the position of the sprite
	FPoint getSpriteScale();  // Return the sprites scaling factor
	void setSpriteScale(FPoint sScale); // set the sprites scaling factor
	void scaleSprite(); // update the sprites width & height

	bool isCollidingWithPerPixel(cSprite * otherSprite);
	bool isCollidingWith(SDL_Rect * b);

	virtual SDL_Surface* getSurface(); // Get the surface with the pixels of this sprite
	virtual SDL_Rect getClippingRect(); // Get the rectangle representing the sprite for collision
	virtual SDL_Rect getBoundingBox(); 
	void renderBoundingBox(SDL_Renderer* theRenderer);
};
#endif