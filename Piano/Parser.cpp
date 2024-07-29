#include "Parser.h"

Parser::Parser(Lexer* l)
{
	cursor_pos = 0;
	lexer = l;
	get_next_token();
}

void Parser::get_next_token()
{
	
	Token token = lexer->tokens[cursor_pos];
	while (token.type == kw.space)
	{
		token = lexer->tokens[cursor_pos];
		cursor_pos++;
	}
	
	current_token = token;
}

void Parser::eat(string type)
{
	if (current_token.type == type)
		get_next_token();
	else
		cout << "invalid syntax" << endl;
}

Node* Parser::program()
{
	vector<Node*> nodes = statement_list();
	Node* program = new Node(kt.program);
	for (Node* node : nodes)
		program->append(node);
	
	return program;
}

vector<Node*> Parser::statement_list()
{
	vector<Node*> statement_list;
	statement_list.push_back(statement());
	while (current_token.type == kw.newline)
	{
		eat(kw.newline);
		statement_list.push_back(statement());
	}

	return statement_list;
}

Node* Parser::statement()
{
	if (current_token.type == kw.def)
		return def_statement();
	else if (current_token.type == kw.word)
		return word();
	
	return new Node;
}

Node* Parser::word()
{
	eat(kw.word);
	return new Node;
}

Node* Parser::def_statement()
{
	Token token = current_token;
	eat(kw.def);
	Node* s = set();
	eat(kw.colon);
	eat(kw.base);
	Node* base = num();
	eat(kw.succ);
	Node* succ = binop();
	Node* def = new Node(token);

	def->append(s);
	def->append(base);
	def->append(succ);

	return def;
}

Node* Parser::binop()
{

	Node* left = factor();
	Token token = current_token;
	eat(token.type);
	Node* right = factor();
	Node* binop = new Node(token);
	binop->append(left);
	binop->append(right);


	return binop;
}

Node* Parser::factor()
{
	Token token = current_token;

	if (token.type == kw.num)
		return num();
	else if (token.type == kw.set)
		return set();
	
	return new Node;
}

Node* Parser::num()
{
	Token token = current_token;
	eat(kw.num);

	return new Node(token);
}

Node* Parser::set()
{
	Token token = current_token;
	eat(kw.set);

	return new Node(token);
}

Node* Parser::parse()
{
	return program();
}
