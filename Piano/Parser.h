#pragma once
#include "Lexer.h"
#include "Node.h"
#include "Parser.h"
#include <vector>
using namespace std;


class Parser
{
	Lexer* lexer;
	Token current_token;
	vector<string> STACK;

public:
	Parser(Lexer* lexer);
	Parser() { lexer = nullptr; current_token = Token(kw.eof, kw.eof); }
	
	Node* parse();
	void eat(string type);

	Node* program();
	vector<Node*> statement_list();
	Node* statement();
	Node* def_statement();
	Node* show_statement();
	Node* empty();
	Node* factor();
	Node* num();
	Node* name();
	Set* set();
	vector<Node*> args();
	Node* expr();
	Node* term();
	Node* pow();

	void error();
};

