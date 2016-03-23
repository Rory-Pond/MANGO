#include "SDL2/SDL.h"
#define GLM_FORCE_RADIANS

//Toggle switchs for keyboard

bool Toggle_1()
{
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	static bool SDL_SCANCODE_1_PRESSED = false;

	if (keystate[SDL_SCANCODE_1])
	{	
		if (!SDL_SCANCODE_1_PRESSED)
		{
			SDL_SCANCODE_1_PRESSED = true;
			return true;
		}
	}else
	{
		SDL_SCANCODE_1_PRESSED = false;
	}
	return false;
}

bool Toggle_2()
{
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	static bool SDL_SCANCODE_2_PRESSED = false;

	if (keystate[SDL_SCANCODE_2])
	{	
		if (!SDL_SCANCODE_2_PRESSED)
		{
			SDL_SCANCODE_2_PRESSED = true;
			return true;
		}
	}else
	{
		SDL_SCANCODE_2_PRESSED = false;
	}
	return false;
}

bool Toggle_3()
{
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	static bool SDL_SCANCODE_3_PRESSED = false;

	if (keystate[SDL_SCANCODE_3])
	{	
		if (!SDL_SCANCODE_3_PRESSED)
		{
			SDL_SCANCODE_3_PRESSED = true;
			return true;
		}
	}else
	{
		SDL_SCANCODE_3_PRESSED = false;
	}
	return false;
}

bool Toggle_4()
{
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	static bool SDL_SCANCODE_4_PRESSED = false;

	if (keystate[SDL_SCANCODE_4])
	{	
		if (!SDL_SCANCODE_4_PRESSED)
		{
			SDL_SCANCODE_4_PRESSED = true;
			return true;
		}
	}else
	{
		SDL_SCANCODE_4_PRESSED = false;
	}
	return false;
}

bool Toggle_SPACE()
{
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	static bool SDL_SCANCODE_SPACE_PRESSED = false;

	if (keystate[SDL_SCANCODE_SPACE])
	{	
		if (!SDL_SCANCODE_SPACE_PRESSED)
		{
			SDL_SCANCODE_SPACE_PRESSED = true;
			return true;
		}
	}else
	{
		SDL_SCANCODE_SPACE_PRESSED = false;
	}
	return false;
}

bool Toggle_LEFT()
{
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	static bool SDL_SCANCODE_LEFT_PRESSED = false;

	if (keystate[SDL_SCANCODE_LEFT])
	{	
		if (!SDL_SCANCODE_LEFT_PRESSED)
		{
			SDL_SCANCODE_LEFT_PRESSED = true;
			return true;
		}
	}else
	{
		SDL_SCANCODE_LEFT_PRESSED = false;
	}
	return false;
}

bool Toggle_RIGHT()
{
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	static bool SDL_SCANCODE_RIGHT_PRESSED = false;

	if (keystate[SDL_SCANCODE_RIGHT])
	{	
		if (!SDL_SCANCODE_RIGHT_PRESSED)
		{
			SDL_SCANCODE_RIGHT_PRESSED = true;
			return true;
		}
	}else
	{
		SDL_SCANCODE_RIGHT_PRESSED = false;
	}
	return false;
}