#include "include/header.hpp"

namespace myMain
{
	SDL_Window* win;
	SDL_Renderer* rend;
	bool running = true;
}


using namespace myMain;
//my headers
#include "include/Debug.hpp"
#include "include/Utilities.hpp"
#include "include/Mesh.hpp"
#include "include/GameObject.hpp"
#include "include/Input.hpp"
//scenes
#include "scenes/scene.hpp"

namespace myMain
{
	//fps limit
	int a = 1;
	int b = 1;
	double delta = 0;
	double FPS = 60;



	void draw()
	{
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);
		SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
		
		//TODO: layers work but I need to change storing system
		//std::sort(std::begin(myScene::boxes), std::end(myScene::boxes), layerSortGameObject);
		for (int u = 0; u < 6; u++)
		{
			myScene::boxes[u].draw();
		}
		SDL_RenderPresent(rend);
	}

	void Quit()
	{
		for (int u = 0; u < 6; u++)
		{
			if (myScene::boxes[u].myMesh->myTexture != NULL)
				SDL_DestroyTexture(myScene::boxes[u].myMesh->myTexture);
		}
		SDL_DestroyWindow(win);
		SDL_Quit();
	}
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	win = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_SHOWN);
	rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	//init Input
	for (int i = 0; i < 322; i++)
	{
		Input::newKeys[i] = false;
		Input::keys[i] = false;
		Input::keysUp[i] = false;
		Input::keysDown[i] = false;
	}
	
	/* initialize random seed: */
	srand (time(NULL));
	
	if (Debug::debugMode)
	{
		Debug::time("Meshes", &myScene::Meshes);
		Debug::time("Scene", &myScene::Scene);
	}
	else
	{
		myScene::Meshes();
		myScene::Scene();
	}
	
	while (running)
	{
		if (!Debug::debugMode)
		{
			a = SDL_GetTicks();
			delta = a - b;
			if (delta > 1000/FPS)
			{
				Input::events();
				myScene::Update();
				draw();
				
				b = a;
			}
		}
		else if (Debug::debugFrames > 0)
		{
			Debug::time("event", &Input::events);
			Debug::time("Update", &myScene::Update);
			Debug::time("draw", &draw);
			Debug::debugFrames--;
		}
	}
	Quit();
	return 0;
}