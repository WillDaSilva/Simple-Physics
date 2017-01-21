#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "Entity.h"
#include <iostream>
#include "Timer.h"
#include "Collision.h"

//Screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//A rect to fill screen with
SDL_Surface* filler;

//The image we will load and show on the screen
SDL_Surface* gXOut = NULL;

CircleGroup testGroup(12);

//Frame delta variables

Timer tmr;
float frameDelta;

//Forward Declarations

bool init();
bool loadMedia();
void close();
void loadSurfaces();
void loadCircles();


int main(int argc, char* args[])
{
	
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Reset timer

			tmr.reset();

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}


				//Set framedelta
				
				frameDelta = tmr.elapsed();
				tmr.reset();

				//physics updates
				
				testGroup.update(frameDelta);

				//Collision resolutions

				for (int i = 0; i < testGroup.GetLength(); i++) {

					if ((testGroup[i].rect.x < 0 && testGroup[i].velocity.x < 0)
						|| (testGroup[i].rect.x + testGroup[i].width > SCREEN_WIDTH && testGroup[i].velocity.x > 0))
					{
						testGroup[i].velocity.x *= -1;
					}
					if ((testGroup[i].rect.y < 0 && testGroup[i].velocity.y < 0)
						|| (testGroup[i].rect.y + testGroup[i].height > SCREEN_HEIGHT && testGroup[i].velocity.y > 0))
					{
						testGroup[i].velocity.y *= -1;
					}
				
				}

				//circle vs circle
				if (std::pow(testGroup[0].radius + testGroup[1].radius, 2) >
					std::pow(testGroup[0].position.x - testGroup[1].position.x, 2) +
					std::pow(testGroup[0].position.y - testGroup[1].position.y, 2))
				{
					CircleVSCircle(&testGroup[0], &testGroup[1]);
					tmr.reset();
					//testGroup[0].update(frameDelta);
					//testGroup[1].update(frameDelta);
				}

				//Fill background
				SDL_BlitSurface(filler, NULL, gScreenSurface, NULL);

				
				
				//draw groups
				testGroup.draw(gScreenSurface);

				//Update the surface
				SDL_UpdateWindowSurface(gWindow);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	loadSurfaces();
	loadCircles();

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load splash image
	gXOut = SDL_LoadBMP("Images/x.bmp");
	if (gXOut == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", "Images/x.bmp", SDL_GetError());
		success = false;
	}

	return success;
}

void close()
{
	//Deallocate surface
	SDL_FreeSurface(gXOut);
	gXOut = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void loadSurfaces() {

	//Fill surface
	filler = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);

	SDL_FillRect(filler, NULL, SDL_MapRGB(filler->format, 0, 0, 255));

}

void loadCircles() {

	testGroup[0] = Circle(1, 1, 100, 100, 250, 125, true,1);
	testGroup[1] = Circle(1, 5, 400, 400, 500, 155, true,2);

}