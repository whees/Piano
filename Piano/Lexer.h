#pragma once
#include "Buffer.h"
#include <ctype.h>
#include <iostream>
using namespace std;

class Lexer
{
public:
	Buffer* buffer;
	int pos;
	char current_char;

	Lexer();
	Lexer(Buffer* b);
	~Lexer();

	Token name();
	Token num();
	Token get_next_token();

	char peek();
	void error();
	void advance();
	void skip_space();

};
