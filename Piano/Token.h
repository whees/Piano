#pragma once
#include "keycolors.h"
#include "SDL.h"
#include "keywords.h"
#include <string>
#include <unordered_map>
using namespace std;

class Token
{
public:
	Token();
	Token(string type, string value);
	~Token();

	string type;
	string value;
};

struct keytokens
{
	Token null = Token();
	Token program = Token(kw.program, kw.program);
};


extern keytokens kt;
extern unordered_map<string, Token> RESERVED_KEYWORDS;
extern unordered_map<string, SDL_Color> SYNTAX_COLORS;


