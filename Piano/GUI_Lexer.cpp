#include "GUI_Lexer.h"

#include "Lexer.h"

GUI_Lexer::GUI_Lexer()
{
	buffer = new Buffer();
	pos = 0;
	current_char = NULL;
}

GUI_Lexer::GUI_Lexer(Buffer* b)
{
	buffer = b;
	pos = 0;

	if (buffer->text.size())
	{
		current_char = buffer->text[pos];
		lex();
	}
	else
		current_char = NULL;

}

GUI_Lexer::~GUI_Lexer()
{
	delete buffer;
}

void GUI_Lexer::error()
{
	throw invalid_argument("Invalid character.");
}

void GUI_Lexer::advance()
{
	if (++pos >= buffer->text.size())
		current_char = NULL;
	else
		current_char = buffer->text[pos];
}
char GUI_Lexer::peek()
{
	int peek_pos = pos + 1;
	if (peek_pos >= buffer->text.size())
		return NULL;
	return buffer->text[peek_pos];
}

char GUI_Lexer::peek_over_space()
{
	int peek_pos = pos;

	while (peek_pos < buffer->text.size())
	{
		if (buffer->text[peek_pos] != ' ')
			return buffer->text[peek_pos];
		peek_pos++;
	}

	return NULL;
}

Token GUI_Lexer::name()
{
	string n = "";

	while (isalnum(current_char))
	{
		n += current_char;
		advance();
	}

	if (RESERVED_KEYWORDS.count(n))
		return RESERVED_KEYWORDS[n];
	if (SCOPE.count(n))
		return SCOPE[n];
	if (peek_over_space() == ':') 
	{
		SCOPE[n] = Token(kw.name, n);
		return SCOPE[n];
	}
		
	return Token(kw.word, n);
}

Token GUI_Lexer::num()
{
	string n = "";

	while (isdigit(current_char))
	{
		n += current_char;
		advance();
	}

	return Token(kw.num, n);
}

Token GUI_Lexer::space()
{
	string s = "";
	while (current_char == ' ')
	{
		s += current_char;
		advance();
	}

	return Token(kw.space, s);
}

Token GUI_Lexer::get_next_token()
{
	while (current_char != NULL)
	{
		if (current_char == ' ')
			return space();

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
			if (current_char == '+')
			{
				if (peek() == ')')
				{
					advance();
					advance();
					return Token(kw.succ, "(+)");

				}
			}
			else if (current_char == ')')
			{
				advance();
				return Token(kw.get, "()");
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

int GUI_Lexer::lex()
{
	Token token = get_next_token();
	while (token.type != kw.eof)
	{
		try
		{
			tokens.push_back(token);
			token = get_next_token();
		}
		catch (invalid_argument& e)
		{
			cerr << e.what() << endl;
			return -1;
		}

	}
	tokens.push_back(token);
	return 0;
}











