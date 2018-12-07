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
bool cSprite::isCollidingWith(cSprite * otherSprite)
{
	// TODO: Only check pixels inside the intersection
	bool isColliding = false;

	// Pull the surfaces from A and B
	SDL_Surface* surfaceA = getSurface();
	SDL_Surface* surfaceB = otherSprite->getSurface();

	// And read the positions too
	SDL_Point posA = { this->getSpritePos().x, this->getSpritePos().y };
	SDL_Point posB = { otherSprite->getSpritePos().x, otherSprite->getSpritePos().y };

	// Get the clipping rect for both the sprites
	SDL_Rect rectA = this->getClippingRect();
	SDL_Rect rectB = otherSprite->getClippingRect();

	// Lock the surfaces so the pixels can be read
	SDL_LockSurface(surfaceA);
	SDL_LockSurface(surfaceB);

	Uint32 *pixelsA = (Uint32 *)surfaceA->pixels;
	Uint32 *pixelsB = (Uint32 *)surfaceB->pixels;

	opaquePixelPositionsA.clear();
	opaquePixelPositionsB.clear();

	std::cout << "Reading opaque pixels from a total pixel count of " << rectA.w * rectA.h << endl;

	for (int x = rectA.x; x < rectA.x + rectA.w; x++) {
		for (int y = rectA.y; y < rectA.y + rectA.h; y++) {

			Uint32 pixel = pixelsA[(y * surfaceA->w) + x];

			Uint8 red, green, blue, alpha;
			SDL_GetRGBA(pixel, surfaceA->format, &red, &green, &blue, &alpha);

			// If this is a non-transparent pixel
			if (alpha > 0) {
				opaquePixelPositionsA.push_back(SDL_Point({ x, y }));
			}
		}
	}

	std::cout << "Reading opaque pixels from a total pixel count of " << rectB.w * rectB.h << endl;

	for (int x = rectB.x; x < rectB.x + rectB.w; x++) {
		for (int y = rectB.y; y < rectB.y + rectB.h; y++) {

			Uint32 pixel = pixelsB[(y * surfaceB->w) + x];

			Uint8 red, green, blue, alpha;
			SDL_GetRGBA(pixel, surfaceB->format, &red, &green, &blue, &alpha);

			// If this is a non-transparent pixel
			if (alpha > 0) {
				opaquePixelPositionsB.push_back(SDL_Point({ x, y }));
			}
		}
	}


	std::cout << "Checking " << opaquePixelPositionsA.size() << " opaque pixels against " << opaquePixelPositionsB.size() << std::endl;

	for (unsigned int i = 0; i < opaquePixelPositionsA.size(); i++) {
		SDL_Point pixelA = opaquePixelPositionsA[i];
		pixelA = pixelA + posA;
		pixelA = pixelA - SDL_Point{rectA.x, rectA.y};

		for (unsigned int i2 = 0; i2 < opaquePixelPositionsB.size(); i2++) {
			SDL_Point pixelB = opaquePixelPositionsB[i2];
			pixelB = pixelB + posB;
			pixelB = pixelB - SDL_Point{ rectB.x, rectB.y };

			std::cout << "Comparing [" << pixelA.x << "," << pixelA.y << "] to [" << pixelB.x << ", " << pixelB.y << "]" << endl;
			if (pixelA.x == pixelB.x && pixelA.y == pixelB.y) {
				isColliding = true;
				continue;
			}
		}

		if (isColliding) continue;
	}

	SDL_UnlockSurface(surfaceA);
	SDL_UnlockSurface(surfaceB);

	std::cout << "isColliding: " << isColliding << endl;
	//delete surfaceA;
	//delete surfaceB;
	//delete pixelsA;

	return isColliding;
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
	SDL_RenderDrawRect(theRenderer, &getBoundingBox());
}
