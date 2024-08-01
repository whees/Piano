#pragma once
#include "SDL.h"

const struct keycolors
{
	SDL_Color black = { 0,0,0 };
	SDL_Color blue = { 50,170,250 };
	SDL_Color green = { 150,250,200 };
	SDL_Color gold = { 250,250,150 };
	SDL_Color lavender = { 200,150,250 };
	SDL_Color white = { 250,250,250 };
};

extern keycolors kc;

