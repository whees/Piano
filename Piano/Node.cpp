#include "Node.h"

Node* make_Num(string value)
{
	return new Num(Token(kw.num, value));
}

Node* make_Name(string value)
{
	return new Name(Token(kw.name, value));
}







