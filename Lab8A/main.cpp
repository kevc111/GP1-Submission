#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


#include <windows.h>
#include "windowOGL.h"
#include "GameConstants.h"
#include "cWNDManager.h"
#include "cInputMgr.h"
#include "cSoundMgr.h"
#include "cFontMgr.h"
#include "cSprite.h"
#include "cAsteroidsGame.h"
#include <MMSystem.h>
#include "cButton.h"


int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR cmdLine,
	int cmdShow)
{

	//Set our window settings
	const int windowWidth = 1024;
	const int windowHeight = 768;
	const int windowBPP = 16;


	//This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();

	bool startup = true;

	//The example OpenGL code
	windowOGL theOGLWnd;

	//Attach our the OpenGL window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);

	//Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
	{
		//If it fails

		MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
		return 1;
	}

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
	{
		MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
		return 1;
	}

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	/* initialize random seed: */
	srand((unsigned int)time(NULL));

	// Create vector array of textures
	LPCSTR texturesToUse[] = { "Images\\Rabbit.png", "Images\\rocketSprite.TCone.png", "Images\\Rabbit.png", "Images\\Rabbit.png" };
	for (int tCount = 0; tCount < 4; tCount++)
	{
		theGameTextures.push_back(new cTexture());
		theGameTextures[tCount]->createTexture(texturesToUse[tCount]);
	}

	// load game sounds
	// Load Sound
	LPCSTR gameSounds[3] = { "Audio/Space_Coast.mp3", "Audio/shot007.wav", "Audio/explosion2.wav" };

	//Plays Background Track
	PlaySound(TEXT("Audio/tune.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);


	theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[1]);
	theSoundMgr->add("Explosion", gameSounds[2]);

	// load game fontss
	// Load Fonts
	LPCSTR gameFonts[2] = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };

	theFontMgr->addFont("SevenSeg", gameFonts[0], 24);
	theFontMgr->addFont("Space", gameFonts[1], 24);

	// Create vector array of textures

	for (int astro = 0; astro < 6; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos(glm::vec2(windowWidth / (10 + 1), windowHeight / (1 + 1)));
		theAsteroids[astro]->setSpriteTranslation(glm::vec2((10), (1 + 1)));
		int randAsteroid = rand() % 4;
		theAsteroids[astro]->setTexture(theGameTextures[randAsteroid]->getTexture());
		theAsteroids[astro]->setTextureDimensions(theGameTextures[randAsteroid]->getTWidth(), theGameTextures[randAsteroid]->getTHeight());
		theAsteroids[astro]->setSpriteCentre();
		theAsteroids[astro]->setAsteroidVelocity(glm::vec2(glm::vec2(0.0f, 0.0f)));
		theAsteroids[astro]->setActive(true);
		theAsteroids[astro]->setMdlRadius();
	}

	// Create vector array of textures
	vector<cTexture*> textureBkgList;
	LPCSTR bkgTexturesToUse[] = { "Images\\MainScreen.jpg", "Images/startScreen.jpg", "Images/EndScreen.jpg" };
	for (int tCount = 0; tCount < 3; tCount++)
	{
		textureBkgList.push_back(new cTexture());
		textureBkgList[tCount]->createTexture(bkgTexturesToUse[tCount]);
	}


	//Sets the Background textures for each state
	cTexture textureBkgd;
	textureBkgd.createTexture("Images\\MainScreen.png");
	cBkGround spriteBkgd;
	spriteBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteBkgd.setTexture(textureBkgd.getTexture());
	spriteBkgd.setTextureDimensions(textureBkgd.getTWidth(), textureBkgd.getTHeight());

	cBkGround spriteStartBkgd;
	spriteStartBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteStartBkgd.setTexture(textureBkgList[1]->getTexture());
	spriteStartBkgd.setTextureDimensions(textureBkgList[1]->getTWidth(), textureBkgList[1]->getTHeight());

	cBkGround spriteEndBkgd;
	spriteEndBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteEndBkgd.setTexture(textureBkgList[2]->getTexture());
	spriteEndBkgd.setTextureDimensions(textureBkgList[2]->getTWidth(), textureBkgList[2]->getTHeight());


	//Sets the texture for the car and inital position of the car
	cTexture rocketTxt;
	rocketTxt.createTexture("Images\\CarSprite.png");
	cRocket rocketSprite;
	rocketSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	rocketSprite.setSpritePos(glm::vec2(10.0f, 300.0f));
	rocketSprite.setTexture(rocketTxt.getTexture());
	rocketSprite.setTextureDimensions(rocketTxt.getTWidth(), rocketTxt.getTHeight());
	rocketSprite.setSpriteCentre();

	// Attach sound manager to rocket sprite
	rocketSprite.attachSoundMgr(theSoundMgr);

	// Create vector array of button textures
	vector<cTexture*> btnTextureList;
	LPCSTR btnTexturesToUse[] = { "Images/Buttons/exitBtn.png", "Images/Buttons/instructionsBtn.png", "Images/Buttons/loadBtn.png", "Images/Buttons/playBtn.png", "Images/Buttons/saveBtn.png", "Images/Buttons/settingsBtn.png" };
	for (int tCount = 0; tCount < 6; tCount++)
	{
		btnTextureList.push_back(new cTexture());
		btnTextureList[tCount]->createTexture(btnTexturesToUse[tCount]);
	}

	//Sets the buttons up
	cButton exitButton;
	exitButton.attachInputMgr(theInputMgr);
	exitButton.setTexture(btnTextureList[0]->getTexture());
	exitButton.setTextureDimensions(btnTextureList[0]->getTWidth(), btnTextureList[0]->getTHeight());

	cButton playButton;
	playButton.attachInputMgr(theInputMgr);
	playButton.setTexture(btnTextureList[3]->getTexture());
	playButton.setTextureDimensions(btnTextureList[3]->getTWidth(), btnTextureList[3]->getTHeight());


	gameState theGameState = MENU;
	btnTypes theBtnType = EXIT;



	//This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
	{
		pgmWNDMgr->processWNDEvents(); //Process any window events

		//We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Changes the state of the game
		switch (theGameState)
		{
		case MENU:

			//Renders the main menu background
			spriteStartBkgd.render();

			//Renders the buttons and their positions
			playButton.setSpritePos(glm::vec2(500.0f, 300.0f));
			exitButton.setSpritePos(glm::vec2(500.0f, 375.0f));
			playButton.render();
			exitButton.render();

			//Pressing the play button will change the state of the game to the main game
			theGameState = playButton.update(theGameState, PLAYING);
			exitButton.update();

			//Renders the text onto the screen
			theFontMgr->getFont("SevenSeg")->printText("Race the Rabbit!", FTPoint(300.0f, -100.0f, 0.0f));
			theFontMgr->getFont("SevenSeg")->printText("Up arrow key to accelerate", FTPoint(1.0f, -200.0f, 0.0f));
			theFontMgr->getFont("SevenSeg")->printText("Down arrow key to brake/reverse", FTPoint(1.0f, -250.0f, 0.0f));
			theFontMgr->getFont("SevenSeg")->printText("Left arrow key to Turn left", FTPoint(1.0f, -300.0f, 0.0f));
			theFontMgr->getFont("SevenSeg")->printText("Right arrow key to turn right", FTPoint(1.0f, -350.0f, 0.0f));
			theFontMgr->getFont("SevenSeg")->printText("Make sure you have maximised your screen!", FTPoint(1.0f, -150.0f, 0.0f));


			//Plays sound effects for when a button is pressed
			if (exitButton.getClicked())
			{
				PlaySound(TEXT("Audio/shot007.wav"), NULL, SND_FILENAME | SND_ASYNC);
				SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			}
			if (playButton.getClicked())
			{
				PlaySound(TEXT("Audio/shot007.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			break;

		case PLAYING:

			//renders the main screen background
			spriteBkgd.render();

			rocketSprite.update(elapsedTime);
			{
				vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
				while (asteroidIterator != theAsteroids.end())
				{
					if ((*asteroidIterator)->isActive() == false)
					{
						asteroidIterator = theAsteroids.erase(asteroidIterator);
					}
					else
					{
						(*asteroidIterator)->update(elapsedTime);
						(*asteroidIterator)->render();
						++asteroidIterator;
					}
				}
			}

			//Renders the player's sprite onto the screen
			rocketSprite.render();


			//Displays the instructions on screen
			theFontMgr->getFont("Space")->printText("Reach the finish line before the", FTPoint(550.0f, -50.0f, 0.0f));
			theFontMgr->getFont("Space")->printText("Rabbit reaches the end of the map", FTPoint(550.0f, -100.0f, 0.0f));
			theFontMgr->getFont("Space")->printText("Once mission is complete press exit.", FTPoint(550.0f, -150.0f, 0.0f));
			theFontMgr->getFont("Space")->printText("Up arrow key to accelerate", FTPoint(1.0f, -400.0f, 0.0f));
			theFontMgr->getFont("Space")->printText("Down arrow key to brake/reverse", FTPoint(1.0f, -450.0f, 0.0f));
			theFontMgr->getFont("Space")->printText("Left arrow key to Turn left", FTPoint(1.0f, -500.0f, 0.0f));
			theFontMgr->getFont("Space")->printText("Right arrow key to turn right", FTPoint(1.0f, -550.0f, 0.0f));


			//Plays sound effect for when the button is clicked
			if (exitButton.getClicked())
			{
				PlaySound(TEXT("Audio/shot007.wav"), NULL, SND_FILENAME | SND_ASYNC);

			}

			// render button and reset clicked to false
			exitButton.setSpritePos(glm::vec2(740.0f, 575.0f));
			exitButton.render();
			theGameState = exitButton.update(theGameState, END);
			break;

		case END:
			//Renders end screen background
			spriteEndBkgd.render();

			//resets the buttons to false as they were pressed last state
			playButton.setClicked(false);
			exitButton.setClicked(false);

			//Plays background music
			PlaySound(TEXT("Audio/tune.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

			//renders buttons and their positions
			playButton.setSpritePos(glm::vec2(350.0f, 450.0f));
			exitButton.setSpritePos(glm::vec2(350.0f, 525.0f));
			playButton.render();
			exitButton.render();


			//Changes game state
			theGameState = playButton.update(theGameState, PLAYING);
			exitButton.update();

			//plays sound effect if button is pressed
			if (exitButton.getClicked())
			{
				PlaySound(TEXT("Audio/shot007.wav"), NULL, SND_FILENAME | SND_ASYNC);
				SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			}

			//displays text
			theFontMgr->getFont("SevenSeg")->printText("Thank you for playing! Press Play to replay.", FTPoint(1.0f, -600.0f, 0.0f));
			theFontMgr->getFont("SevenSeg")->printText("Game Over", FTPoint(400.0f, -200.0f, 0.0f));

			break;

		}
		pgmWNDMgr->swapBuffers();
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);
	}


	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

	return 0; //Return success
}
