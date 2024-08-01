#include "Lexer.h"

Lexer::Lexer()
{
	buffer = new Buffer();
	pos = 0;
	current_char = buffer->text[pos];
}

Lexer::Lexer(Buffer* b)
{
	buffer = b;
	pos = 0;
	current_char = buffer->text[pos];
}

Lexer::~Lexer() 
{
	delete buffer;
}

void Lexer::error()
{
	throw invalid_argument("Invalid character.");
}

void Lexer::advance()
{
	if (++pos >= buffer->text.size())
		current_char = NULL;
	else
		current_char = buffer->text[pos];
}
char Lexer::peek()
{
	int peek_pos = pos + 1;
	if (peek_pos >= buffer->text.size())
		return NULL;
	return buffer->text[peek_pos];
}

Token Lexer::name()
{
	string result = "";

	while (isalnum(current_char))
	{
		result += current_char;
		advance();
	}

	if (RESERVED_KEYWORDS.count(result))
		return RESERVED_KEYWORDS[result];

	if (current_char == '(')
	{
		advance();
		if (current_char == ')')
		{
			advance();
			return Token(kw.get, result);
		}
		if (current_char == '+' && peek() == ')')
		{
			advance();
			advance();
			return Token(kw.succ, result);
		}
		if (isdigit(current_char))
			return Token(kw.getn, result);
		
		error();
	}

	return Token(kw.name, result);
}

Token Lexer::num()
{
	string result = "";

	while (isdigit(current_char))
	{
		result += current_char;
		advance();
	}

	return Token(kw.num, result);
}

void Lexer::skip_space()
{

	while (current_char == ' ')
		advance();
}

Token Lexer::get_next_token()
{
	while (current_char != NULL)
	{
		if (current_char == ' ')
		{
			skip_space();
			continue;
		}

		if (isalpha(current_char))
			return name();

		if (isdigit(current_char))
			return num();

		if (current_char == '=')
		{
			advance();
			return Token(kw.assign, "=");
		}

		if (current_char == ';')
		{
			advance();
			return Token(kw.semi, ";");
		}
		
		if (current_char == '+')
		{
			advance();
			return Token(kw.add, "+");
		}

		if (current_char == '-')
		{
			advance();
			return Token(kw.sub, "-");
		}

		if (current_char == '*')
		{
			advance();
			return Token(kw.mul, "*");
		}

		if (current_char == '/')
		{
			advance();
			return Token(kw.div, "/");
		}

		if (current_char == '{')
		{
			advance();
			return Token(kw.lbrack, "{");
		}

		if (current_char == '}')
		{
			advance();
			return Token(kw.rbrack, "}");
		}

		if (current_char == '(')
		{
			advance();

			if (current_char == ')')
			{
				advance();
				return Token(kw.get, "()");
			}
			if (current_char == '+' && peek() == ')')
			{
				advance();
				advance();
				return Token(kw.succ, "(+)");
			}

			return Token(kw.lparen, "(");
		}
		
		if (current_char == ')')
		{
			advance();
			return Token(kw.rparen, ")");
		}

		if (current_char == '|')
		{
			advance();
			return Token(kw.pipe, "|");
		}

		if (current_char == ':')
		{
			advance();
			return Token(kw.def, ":");
		}

		if (current_char == '^')
		{
			advance();
			return Token(kw.pow, "^");
		}
		
		error();
	}

	return Token(kw.eof, kw.eof);
}











