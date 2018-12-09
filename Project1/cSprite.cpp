/*
=================
cSprite.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cSprite.h"
/*
=================
- Data constructor initializes the cSprite to the data passed to
- the constructor from the paramater sPosition.
- Is always called, thus ensures all sprite objects are in a consistent state.
=================
*/
cSprite::cSprite() 			// Default constructor
{

	this->spritePos_2D = { 0, 0, 0, 0 };
	this->spriteTexture = NULL;
	this->spriteCentre = {0, 0};
	this->spriteScale = { 1, 1 };
}

cSprite::cSprite(cTexture* theSpriteTexture) 			// Default constructor
{
	this->spriteTexture = theSpriteTexture;
	this->setSpriteDimensions(this->spriteTexture->getTWidth(), this->spriteTexture->getTHeight());
	this->spritePos_2D = { 0, 0, this->spriteTexture->getTWidth(), this->spriteTexture->getTHeight() };
	this->spriteCentre = { this->spritePos_2D.w / 2, this->spritePos_2D.h / 2};
	this->spriteScale = { 1, 1 };
}

/*
=================
- Destructor
=================
*/
cSprite::~cSprite()			// Destructor
{
}
/*
=================
- Return the sprites current position.
=================
*/

SDL_Rect cSprite::getSpritePos()  // Return the sprites current position
{
	return this->spritePos_2D;
}

/*
=================
- set the position of the sprite.
=================
*/

void cSprite::setSpritePos(SDL_Rect sPosition)  // set the position of the sprite
{
	this->spritePos_2D.x = sPosition.x;
	this->spritePos_2D.y = sPosition.y;
	this->spritePos_2D.w = sPosition.w;
	this->spritePos_2D.h = sPosition.h;
}

/*
=================
- Return the sprites current image.
=================
*/

cTexture* cSprite::getTexture()  // Return the sprites current image
{
	return this->spriteTexture;
}

/*
=================
- set the image of the sprite.
=================
*/

void cSprite::setTexture(cTexture* theSpriteTexture)  // set the image of the sprite
{
	this->spriteTexture = theSpriteTexture;
	this->setSpriteDimensions(spriteTexture->getTWidth(), spriteTexture->getTHeight());
	this->spritePos_2D.w = textureWidth;
	this->spritePos_2D.h = textureHeight;
	this->spriteCentre = { this->spritePos_2D.w / 2, this->spritePos_2D.h / 2 };
}


void cSprite::render(SDL_Renderer* theRenderer, SDL_Rect* theSourceRect, SDL_Rect* theDestRect, FPoint theScaling)
{
	this->spriteTexture->renderTexture(theRenderer, this->spriteTexture->getTexture(), theSourceRect, theDestRect, theScaling);

	//renderBoundingBox(theRenderer);
}

void cSprite::render(SDL_Renderer* theRenderer, SDL_Rect* theSourceRect, SDL_Rect* theDestRect, double rotAngle, SDL_Point* spriteCentre, FPoint theScaling)
{
	this->spriteTexture->renderTexture(theRenderer, this->spriteTexture->getTexture(), theSourceRect, theDestRect, rotAngle, spriteCentre, theScaling);
}
/*
=================
- Set the sprite dimensions.
=================
*/
void cSprite::setSpriteDimensions(int texWidth, int textHeight)
{
	this->textureWidth = texWidth;
	this->textureHeight = textHeight;
	this->spriteDimensions = { 0, 0, texWidth, textHeight };
}

/*
=================
- Get the sprite dimensions.
=================
*/
SDL_Rect cSprite::getSpriteDimensions()
{
	return this->spriteDimensions;
}
/*
=================
- Return the sprite centre.
=================
*/

SDL_Point cSprite::getSpriteCentre()  // Return the sprites current position
{
	return this->spriteCentre;
}

/*
=================
- set the the sprite centre.
=================
*/

void cSprite::setSpriteCentre(SDL_Point sCentre)  // set the position of the sprite
{
	this->spriteCentre.x = sCentre.x;
	this->spriteCentre.y = sCentre.y;
}
/*
=================
- Return the sprite scaling.
=================
*/

FPoint cSprite::getSpriteScale()  // Return the sprites current scaling
{
	return this->spriteScale;
}

/*
=================
- set the the sprite scale.
=================
*/

void cSprite::setSpriteScale(FPoint sScale)  // set the sprites current scaling
{
	this->spriteScale.X += sScale.X;
	this->spriteScale.Y += sScale.Y;
}
/*
=================
- Update sprite scale.
=================
*/

void cSprite::scaleSprite()  // set the sprites current scaling
{
	// Scale sprite
	this->spritePos_2D.w = (int)(this->spriteDimensions.w * this->spriteScale.X);
	this->spritePos_2D.h = (int)(this->spriteDimensions.h * this->spriteScale.Y);
	// Scale Sprite Centre for rotation.
	this->spriteCentre.x = this->spritePos_2D.w / 2;
	this->spriteCentre.y = this->spritePos_2D.h / 2;
}

SDL_Surface * cSprite::getSurface()
{
	return this->getTexture()->surface;
}

SDL_Rect cSprite::getClippingRect()
{
	return SDL_Rect({ 0, 0, this->spriteDimensions.w, this->spriteDimensions.h });
}

// Is this sprite colliding with another sprite, using per-pixel collision detection?
bool cSprite::isCollidingWithPerPixel(cSprite * otherSprite)
{
	// Start by getting the two boiunding boxes
	SDL_Rect boundingA = this->getBoundingBox();
	SDL_Rect boundingB = otherSprite->getBoundingBox();

	// Calculate intersection between bounding boxes of both
	SDL_IntersectRect(&boundingA, &boundingB, &intersectRect);

	// Pull the surfaces from A and B
	SDL_Surface* surfaceA = getSurface();
	SDL_Surface* surfaceB = otherSprite->getSurface();

	// And read the positions too
	SDL_Point posA = { this->getSpritePos().x, this->getSpritePos().y };
	SDL_Point posB = { otherSprite->getSpritePos().x, otherSprite->getSpritePos().y };

	// Get the clipping rect for both the sprites
	SDL_Rect rectA = this->getClippingRect();
	SDL_Rect rectB = otherSprite->getClippingRect();

	// And grab the raw pixels from the surfaces
	Uint32 *pixelsA = (Uint32 *)surfaceA->pixels;
	Uint32 *pixelsB = (Uint32 *)surfaceB->pixels;

	// To store the pixel data in
	Uint8 red, green, blue, alpha;

	// For every pixel in the intersected rectangle...
	for (int x = intersectRect.x; x < intersectRect.x + intersectRect.w; x++) {
		for (int y = intersectRect.y; y < intersectRect.y + intersectRect.h; y++) {
			// Grab the pixels in that position from both textures, factoring in world position and the current frame if animated
			Uint32 thisPixel = pixelsA[((y - posA.y - rectA.y) * surfaceA->w) + (x - posA.x - rectA.x)];
			Uint32 otherPixel = pixelsB[((y - posB.y - rectB.y) * surfaceB->w) + (x - posB.x - rectB.x)];

			// Check alpha for first pixel
			SDL_GetRGBA(thisPixel, surfaceB->format, &red, &green, &blue, &alpha);

			if (alpha > 0) {
				// Check alpha for second pixel
				SDL_GetRGBA(otherPixel, surfaceB->format, &red, &green, &blue, &alpha);

				// If they're both in the same position and are opaque, they are colliding. Early exit true
				if (alpha > 0) {
					return true;
				}

			}
		}
	}


	// No intersection was found
	return false;
}



// Collision with bounding box
bool cSprite::isCollidingWith(SDL_Rect * b)
{
	SDL_Rect a = getBoundingBox();

	return SDL_HasIntersection(&a, b);
}

SDL_Rect cSprite::getBoundingBox()
{
	return SDL_Rect { this->spritePos_2D.x, this->spritePos_2D.y, this->spriteDimensions.w, this->spriteDimensions.h };
}

void cSprite::renderBoundingBox(SDL_Renderer * theRenderer)
{
	SDL_RenderDrawRect(theRenderer, &intersectRect);
	//SDL_RenderDrawRect(theRenderer, &getBoundingBox());
}
