#include "Buffer.h"


Buffer::Buffer()
{
	text="";
	cursor_pos = 0;
}

Buffer::~Buffer()
{}

void Buffer::type(string newtext)
{
	text = text.insert(cursor_pos,newtext);
	cursor_pos+= newtext.size();
}

void Buffer::new_line()
{
	text += ';';
	cursor_pos++;
}

void Buffer::backspace()
{
	if (cursor_pos > 0)
	{
		text.erase(cursor_pos - 1,1);
		cursor_pos--;
	}
}

void Buffer::move_cursor_right()
{
	int prop_pos = ++cursor_pos;

	if (prop_pos > text.size())
		cursor_pos = text.size();
	else
		cursor_pos = prop_pos;
}

void Buffer::move_cursor_left()
{
	int prop_pos = --cursor_pos;

	if (prop_pos < 0)
		cursor_pos = 0;
	else
		cursor_pos = prop_pos;
}






