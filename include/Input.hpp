#pragma once

#define MouseLeft 323
#define MouseMiddle 324
#define MouseRight 325
#define Horizontal 1
#define Vertical 2
#define VerticalAndHorizontal 3
#define MouseX 4
#define MouseY 5
#define MouseXY 6
#define MousePos 7
#define WheelY 8
#define WheelX 9

namespace Input
{
	bool newKeys[325];
	bool keysUp[325];
	bool keysDown[325];
	bool keys[325];
	
	int horizontalInput;
	int verticalInput;
	
	float mouseWheelY;
	float mouseWheelX;
	
	vec2 mouseXY;
	vec2 lastMousePos;
	
	void events()
	{	
		mouseWheelY = 0;
		mouseWheelX = 0;
		
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				running = false;
			
			//get pressed and unpressed keys
			else if (event.type == SDL_KEYDOWN)
				newKeys[event.key.keysym.sym] = true;
			
			else if (event.type == SDL_KEYUP)
				newKeys[event.key.keysym.sym] = false;
			
			else if (event.type == SDL_MOUSEBUTTONDOWN)
				newKeys[event.button.button+322] = true;
			
			else if (event.type == SDL_MOUSEBUTTONUP)
				newKeys[event.button.button+322] = false;
			
			//mouse wheel
			if(event.type == SDL_MOUSEWHEEL)
			{
				mouseWheelY = event.wheel.y;
				mouseWheelX = event.wheel.x;
			}
		}
		//assign keys
		for (int i = 0; i < 325; i++)
		{
			keysDown[i] = false;
			keysUp[i] = false;
			
			if (keys[i] == false && newKeys[i] == true)
				keysDown[i] = true;
			if (keys[i] == true && newKeys[i] == false)
				keysUp[i] = true;
			
			keys[i] = newKeys[i];
		}
		//mouse delta
		int currMouseX;
		int currMouseY;
		SDL_GetMouseState(&currMouseX, &currMouseY);
		mouseXY = vec2(currMouseX, currMouseY) - lastMousePos;
		lastMousePos = vec2(currMouseX, currMouseY);
		
		//vertical/horizontal Input
		verticalInput = 0;
		horizontalInput = 0;
		
		if (keys[SDLK_w])
			verticalInput += 1;
		if (keys[SDLK_s])
			verticalInput -= 1;
		if (keys[SDLK_a])
			horizontalInput -= 1;
		if (keys[SDLK_d])
			horizontalInput += 1;
	}
	
	int GetAxisInt(int choice)
	{
		switch(choice)
		{
			case 1:
				return horizontalInput;
			case 2:
				return verticalInput;
			case 4:
				return mouseXY.x;
			case 5:
				return mouseXY.y;
			case 8:
				return mouseWheelX;
			case 9:
				return mouseWheelY;
			default:
				break;
		}
		std::cout << "wrong GetAxis" << std::endl;
		return 0;
	}
	
	float GetAxisFloat(int choice)
	{
		switch(choice)
		{
			case 1:
				return horizontalInput;
			case 2:
				return verticalInput;
			case 4:
				return mouseXY.x;
			case 5:
				return mouseXY.y;
			case 8:
				return mouseWheelX;
			case 9:
				return mouseWheelY;
			default:
				break;
		}
		std::cout << "wrong GetAxis" << std::endl;
		return 0;
	}
	
	vec2 GetAxisVec2(int choice)
	{
		switch(choice)
		{
			case 3:
				return vec2(horizontalInput, verticalInput);
			case 6:
				return mouseXY;
			case 7:
				return lastMousePos;
			default:
				break;
		}
		std::cout << "wrong GetAxis" << std::endl;
		return vec2(0, 0);
	}
}