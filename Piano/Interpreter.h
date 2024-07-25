#pragma once
#include "Node.h"
#include "Parser.h"



class Interpreter : public Visitor
{
public:
	Interpreter(Parser* p) {  parser = p; }
	Interpreter() { parser = new Parser; }
	~Interpreter() {}

	void interpret() { visit(parser->parse()); }
	Parser* parser;
};

