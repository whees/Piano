#pragma once
#include "Lexer.h"
#include "Node.h"
#include "Parser.h"
using namespace std;


class Parser
{
	int cursor_pos;
	Lexer* lexer;
	Token current_token;
public:
	Parser(Lexer* lexer);
	Parser() { cursor_pos = 0; lexer = new Lexer; }
	
	Node* parse();
	void get_next_token();
	void eat(string type);

	Node* program();
	vector<Node*> statement_list();
	Node* statement();
	Node* def_statement();
	Node* binop();
	Node* factor();
	Node* num();
	Node* set();
	Node* word();
};

