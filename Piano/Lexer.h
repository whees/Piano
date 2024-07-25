#pragma once
#include "Buffer.h"
#include <vector>
#include <iostream>
using namespace std;

class Lexer
{
public:
	vector<Token> tokens;
	Buffer* buffer;
	int cursor_pos;
	int token_pos;
	int word_pos;

	Lexer();
	~Lexer();

	void lex();
	Token get_newline(int* pos);
	Token get_word(int* pos);
	Token get_space(int* pos);
	Token get_id(int* pos);

	void move_cursor_left();
	void move_cursor_right();
	void get_pos();
};
