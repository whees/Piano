#pragma once
#include "Lexer.h"
#include "Interpreter.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
using namespace std;

class GUI
{
public:
	const int font_size = 30;
	int clock;
	bool show_cursor;
	bool running = true;


	Lexer* lexer;
	Parser* parser;
	Interpreter* interpreter;

	TTF_Font* font;
	TTF_Font* boldfont;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Rect cursor_rect;

	GUI(int argc, char* argv[]);
	~GUI();

	bool work_flow();
	void handle_events();
	void render();
	void render_buffer();
	void render_cursor();
	void interpret();
};

