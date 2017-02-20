// Include important C++ libraries here.
#include "stdafx.h"
#include <sstream>
#include <SFML/Graphics.hpp>

using namespace sf;

void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

// Where is the player/branch?
// Left or Right
enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

int main()
{
	// Create a video mode object
	VideoMode vm(1920, 1080);
	// Create and open a window for the game RenderWindow
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
	
	// Create a texture to hold a graphic in the GPU
	Texture textureBackground;
	// Load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");
	//Create a sprite
	Sprite spriteBackground;
	// Attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);
	// Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);
	
	// Make a tree sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	// Prepare the bee sprite
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);

	// Is the bee currently moving?
	bool beeActive = false;

	// How fast can the bee fly
	float beeSpeed = 0.0f;

	// Make 3 cloud sprites from 1 texture
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	//Sprite spriteCloud1, spriteCloud2, spriteCloud3;
	Sprite spriteClouds[3];
	bool cloudActive[3]{ false,false,false };
	float cloudSpeed[3]{ 0.0f,0.0f,0.0f };
	
	for (int i = 0; i < 3; i++) {
		spriteClouds[i].setTexture(textureCloud);
		spriteClouds[i].setPosition(0, i * 250);
	}
	/*bool cloud1Active = false, cloud2Active = false, cloud3Active = false;

	float cloud1Speed = 0.0f, cloud2Speed = 0.0f, cloud3Speed = 0.0f;*/

	Clock clock;

	// Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;

	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// Track whether the game is running
	bool paused = true;

	// Draw some text
	int score = 0;

	Text messageText;
	Text scoreText;

	// We need to choose a font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	// Set the font to our message
	messageText.setFont(font);
	scoreText.setFont(font);

	// Assign the actual message
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");

	// Make it really big
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	// Choose a color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	// Position the text
	auto textRect = messageText.getLocalBounds();

	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

	scoreText.setPosition(20, 20);

	// Prepare 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	// Set the texture for each branch sprite
	for (auto branch : branches) {
		branch.setTexture(textureBranch);
		branch.setPosition(-2000, -2000);
		// Set the sprite's origin to dead center
		// We can then spin it around without changing its position
		branch.setOrigin(220, 20);
	}

	while (window.isOpen())
	{
		/*****************************************************
		 * Handle the players input                          *
		 *****************************************************/
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Start the game
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;

			// Reset the time and the score
			score = 0;
			timeRemaining = 5;
		}

		/****************************************************
		 * Update the scene                                 *
		*****************************************************/
		if (!paused)
		{
			// Measure time
			Time dt = clock.restart();

			// Subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();
			// size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				// Puase the game
				paused = true;

				// Change the message shown to the player
				messageText.setString("Out of time!!");

				// Reposition the text based on its new size
				auto textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
			}

			// Setup the bee
			if (!beeActive)
			{
				// How fast is the bee
				srand((int)time(0));
				beeSpeed = (rand() % 200) + 200;

				// How high is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else
			{
				// Move the bee
				auto beePosition = spriteBee.getPosition();
				spriteBee.setPosition(beePosition.x - (beeSpeed * dt.asSeconds()), beePosition.y);

				// Has the bee reached the right hand edge of the screen?
				if (spriteBee.getPosition().x < -100)
				{
					// Set it up ready to be a whole new bee next frame
					beeActive = false;
				}
			}

			// Manage clouds
			for (int i = 0; i < 3; i++) {
				if (!cloudActive[i])
				{
					// How fast is the cloud
					srand((int)time(0) * 10 * (i + 1));
					cloudSpeed[i] = (rand() % 200);

					// how high is the cloud
					srand((int)time(0) * 10);
					float height = (rand() % (150 + i * 150));
					if (i > 0) {
						height -= 150;
					}
					spriteClouds[i].setPosition(-200, height);
					cloudActive[i] = true;
				}
				else
				{
					auto cloudPosition = spriteClouds[i].getPosition();
					spriteClouds[i].setPosition(cloudPosition.x + (cloudSpeed[i] * dt.asSeconds()), cloudPosition.y);

					// Has the cloud reached the right hand side of the screen?
					if (spriteClouds[i].getPosition().x > 1920)
					{
						// Set it up ready to be a whole new cloud next frame
						cloudActive[i] = false;
					}
				}
			}


			// Update the score text
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			// update the branch sprites
			for (auto i = 0; i < NUM_BRANCHES; i++) {
				float height = i * 150;
				if (branchPositions[i] == side::LEFT) {
					// Move the sprite to the left side
					branches[i].setPosition(610, height);

					// Flip the sprite around the other way
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT) {
					// Move the sprite to the right side
					branches[i].setPosition(1330, height);

					// Set the sprite rotation to normal
					branches[i].setRotation(0);
				}
				else {
					// Hide the branch
					branches[i].setPosition(3000, height);
				}
			}
		}

		/****************************************************
		 * Draw the scene                                   *
		 ****************************************************/
		// Clear everything from the last frame
		window.clear();

		// Draw our game scene here
		window.draw(spriteBackground);
		
		// Draw the clouds
		for (auto cloud : spriteClouds) {
			window.draw(cloud);
		}

		for (auto branch : branches) {
			window.draw(branch);
		}
		// Draw a tree
		window.draw(spriteTree);

		// Draw the insect
		window.draw(spriteBee);

		//Draw the score
		window.draw(scoreText);

		// Draw the time bar
		window.draw(timeBar);

		if (paused)
		{
			// Draw the message
			window.draw(messageText);
		}
		// Show everything we just drew
		window.display();
	}
    return 0;
}

void updateBranches(int seed) {
	// Move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		branchPositions[j] = branchPositions[j - 1];
	}

	// Spawn a new branch at position 0
	// LEFT, RIGHT, or NONE
	srand((int)time(0) + seed);
	int r = rand() % 5;
	switch (r) {
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}

