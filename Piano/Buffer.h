#pragma once
#include "Token.h"
#include <iostream>
using namespace std;

class Buffer
{
public:
	string text;
	int cursor_pos;

	Buffer();
	~Buffer();

	void type(string newtext);
	void new_line();
	void backspace();
	void move_cursor_left();
	void move_cursor_right();
};

