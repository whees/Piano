#include "Token.h"

Token::Token(string typ, string val)
{
	type = typ;
	value = val;
}

Token::Token()
{
	type = kw.null;
	value = kw.null;
}

Token::~Token() {}


keytokens kt = keytokens();

map<string, Token> RESERVED_KEYWORDS = {	{kw.def,Token(kw.word,kw.def)},
											{kw.base,Token(kw.word,kw.base)}, 
											{kw.succ,Token(kw.word,kw.succ)} };

map<string, SDL_Color> SYNTAX_COLORS = {	{kw.def,kc.lavender},
											{kw.id,kc.green },
											{kw.base,kc.gold},
											{kw.succ,kc.gold},
											{kw.ref,kc.gold} };




