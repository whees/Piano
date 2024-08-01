#pragma once
#include "Buffer.h"
#include <ctype.h>
#include <vector>
#include <iostream>
using namespace std;


class GUI_Lexer
{
public:
	Buffer* buffer;
	vector<Token> tokens;
	unordered_map<string, Token> SCOPE;

	int pos;
	char current_char;

	GUI_Lexer();
	GUI_Lexer(Buffer* b);
	~GUI_Lexer();

	Token space();
	Token name();
	Token num();
	Token get_next_token();

	char peek();
	char peek_over_space();
	void error();
	void advance();
	int lex();
};

