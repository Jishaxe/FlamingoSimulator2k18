#include "cScoreManager.h"



cScoreManager::cScoreManager()
{
}


cScoreManager::~cScoreManager()
{
}

void cScoreManager::render(SDL_Renderer * theRenderer)
{
	// Build up the score text, padding both the score and the high score with leading 0s
	stringstream text;
	text.fill('0');
	text.width(6);
	text << currentScore;
	text << " HI ";

	text.fill('0');
	text.width(6);
	text << highScore;

	// Generate the score texture using the font provided
	SDL_Texture* scoreTexture = cFontMgr::getInstance()->getFont("unifont")->createTextTexture(theRenderer, text.str().c_str(), textType::solid, { 50, 50, 50, 255 }, { 0, 0, 0, 0 });
	int width;
	int height;
	SDL_QueryTexture(scoreTexture, NULL, NULL, &width, &height); // determine the width an height of the texture
	SDL_Rect position = { WINDOW_WIDTH - width - 15, 10, width, height };

	// Manually render the texture
	SDL_RenderCopy(theRenderer, scoreTexture, NULL, &position);
}

void cScoreManager::update(double deltaTime)
{
}

void cScoreManager::saveScore()
{
	ofstream scoreFile;
	scoreFile.open("highscore.dat", ios::trunc); // Overwrites existing file
	scoreFile << highScore;
	scoreFile.close();
}

void cScoreManager::loadScore()
{
	string score;
	ifstream scoreFile("highscore.dat");
	if (scoreFile.is_open())
	{
		getline(scoreFile, score);
		scoreFile.close();
		highScore = stoi(score); // Converts string to int
	}
	else {
		highScore = 0; // No file, just default to 0
	}
}
