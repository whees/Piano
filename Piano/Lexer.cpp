#include "Lexer.h"

Lexer::Lexer()
{
	buffer = new Buffer();
	cursor_pos = 0;
	token_pos = 0;
	word_pos = 0;
}

Lexer::~Lexer() 
{
	delete buffer;
}

void Lexer::lex()
{
	tokens.clear();
	int pos = 0;

	while (pos < buffer->text.size())
	{
		char glyph = buffer->text[pos];
		Token token;
		
		switch (glyph)
		{
		case ';':
			token = get_newline(&pos);
			break;
		case ' ':
			token = get_space(&pos);
			break;
		case '\\':
			token = get_id(&pos);
			break;
		default:
			token = get_word(&pos);
			break;
		}

		tokens.push_back(token);
	}
	tokens.push_back(Token(kw.eof, kw.eof));
	get_pos();
}

Token Lexer::get_newline(int* pos)
{
	(*pos)++;
	return Token(kw.newline, ";");
}

Token Lexer::get_space(int* pos)
{
	string space(1, buffer->text[*pos]);

	while (++(*pos) < buffer->text.size())
	{
		if (buffer->text[*pos] != ' ')
			break;
		space += buffer->text[*pos];
	}

	return Token(kw.space, space);
}

Token Lexer::get_word(int* pos)
{
	string word(1, buffer->text[*pos]);
	
	while (++(*pos) < buffer->text.size())
	{
		if (buffer->text[*pos] == ' ' || buffer->text[*pos] == ';' || buffer->text[*pos] == ':')
			break;
		word += buffer->text[*pos];
	}

	if (RESERVED_KEYWORDS.count(word))
		return RESERVED_KEYWORDS[word];

	return Token(kw.word, word);
}

Token Lexer::get_id(int* pos)
{
	string id(1, buffer->text[*pos]);

	while (++(*pos) < buffer->text.size())
	{
		if (buffer->text[*pos] == ' ' || buffer->text[*pos] == ';' || buffer->text[*pos] == ':')
			break;
		id += buffer->text[*pos];
	}

	if (id.size() == 2)
		return Token(kw.id, id);
	
	return Token(kw.word, id);
}

void Lexer::get_pos()
{
	cursor_pos = 0;
	token_pos = 0;
	word_pos = 0;
	if (!tokens.size())
		return;
	int pos = 0;

	while(token_pos < tokens.size())
	{
		Token token = tokens[token_pos];

		if (token.type == kw.word || token.type == kw.space || token.type == kw.newline)
		{
			word_pos = 0;
			while (word_pos < token.value.size() && pos < buffer->cursor_pos)
			{
				pos++;
				word_pos++;
				cursor_pos++;
			}
			if (pos == buffer->cursor_pos)
				break;
		}
		else if (token.type == kw.id)
		{
			word_pos = 0;
			while (word_pos < token.value.size() && pos < buffer->cursor_pos)
			{
				pos++;
				word_pos++;
			}


			cursor_pos++;

			if (pos == buffer->cursor_pos)
				break;
		}

		token_pos++;
	}
}

void Lexer::move_cursor_left()
{
	int d = tokens[token_pos].type == kw.id ? 2 : 1;
	for (int i = 0; i < d; i++)
		buffer->move_cursor_left();

}

void Lexer::move_cursor_right()
{

	if (token_pos < tokens.size())
	{
		int d = tokens[token_pos + 1].type == kw.id ? 2 : 1;
		for (int i = 0; i < d; i++)
			buffer->move_cursor_right();
	}

}




