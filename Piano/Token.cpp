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

unordered_map<string, Token> RESERVED_KEYWORDS = { {kw.show,Token(kw.show,kw.show)}};

unordered_map<string, SDL_Color> SYNTAX_COLORS = {	{kw.name,kc.lavender },
													{kw.num, kc.green},
													{kw.succ,kc.lavender},
													{kw.get,kc.lavender},
													{kw.show,kc.gold} };




