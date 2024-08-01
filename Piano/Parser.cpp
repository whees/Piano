#include "Parser.h"

Parser::Parser(Lexer* l)
{
	lexer = l;
	current_token = lexer->get_next_token();
}

void Parser::error()
{

}

void Parser::eat(string type)
{
	if (current_token.type == type)
		current_token = lexer->get_next_token();
	else
		error();
}

Node* Parser::program()
{
	vector<Node*> nodes = statement_list();
	Program* program = new Program();
	for (Node* node : nodes)
		program->append(node);
	
	return program;
}

vector<Node*> Parser::statement_list()
{
	vector<Node*> statement_list;
	statement_list.push_back(statement());
	while (current_token.type == kw.semi)
	{
		eat(kw.semi);
		statement_list.push_back(statement());
	}

	return statement_list;
}

Node* Parser::statement()
{
	if (current_token.type == kw.name)
		return def_statement();
	if (current_token.type == kw.show)
		return show_statement();

	return empty();
}

Node* Parser::def_statement()
{
	Node* nam = name();
	STACK.push_back(nam->token.value);
	Token token = current_token;
	eat(kw.def);
	Set* s = set();
	Define* define = new Define(nam,s);
	STACK.pop_back();
	return define;
}

Node* Parser::show_statement()
{
	eat(kw.show);
	Node* arg = factor();
	Node* show = new Show(arg);
	return show;
}

Node* Parser::name()
{
	Name* name_ = new Name(current_token);
	eat(kw.name);
	return name_;
}

Node* Parser::num()
{
	Num* num_ = new Num(current_token);
	eat(kw.num);
	return num_;
}

vector<Node*> Parser::args()
{
	vector<Node*> args_;
	args_.push_back(expr());

	if (current_token.type == kw.pipe)
	{
		eat(kw.pipe);
		args_.push_back(expr());
	}

	return args_;
}

Set* Parser::set()
{
	eat(kw.lbrack);
	vector<Node*> arg = args();

	Node* base = new Num(Token(kw.num, "0"));
	Node* succ = new NoOp;
	if (arg.size() == 1)
	{
		succ = arg[0];
	}
	else if (arg.size() == 2)
	{
		if (arg[0]->token.type == kw.num)
		{
			base = arg[0];
			succ = arg[1];
		}
		else
		{
			base = arg[1];
			succ = arg[0];
		}
	}
	else
		error();

	Set* set = new Set(base,succ);
	eat(kw.rbrack);
	return set;
}

Node* Parser::empty()
{
	Node* empty = new NoOp;

	return empty;
}

Node* Parser::factor()
{
	Token token = current_token;

	if (token.type == kw.num)
		return num();
	if (token.type == kw.name)
		return name();
	if (token.type == kw.lparen)
	{
		eat(kw.lparen);
		Node* node = expr();
		eat(kw.rparen);
		return node;
	}
	if (token.type == kw.get)
	{
		Node* name_ = token.value == "()" ? new Self() : make_Name(token.value);
		eat(kw.get);	
		return new Get(name_);
	}
	if (token.type == kw.succ)
	{
		Node* name_ = token.value == "(+)" ? new Self() : make_Name(token.value);
		eat(kw.succ);
		return new Succ(name_);
	}
	if (token.type == kw.getn)
	{
		Node* name_ = make_Name(token.value);
		eat(kw.getn);
		Node* num_ = num();
		eat(kw.rparen);
		return new GetN(name_,num_);
	}
	if (token.type == kw.add)
	{
		eat(kw.add);
		UnOp* unop = new UnOp(token,factor());

		return unop;
	}
	if (token.type == kw.sub)
	{
		eat(kw.sub);
		Node* unop = new UnOp(token,factor());

		return unop;
	}

	error();
	return new NoOp;
}

Node* Parser::pow()
{
	Node* node = factor();

	while (current_token.type == kw.pow)
	{
		Token token = current_token;
		eat(kw.pow);
		node = new BinOp(token, node, factor());
		
	}

	return node;
}

Node* Parser::term()
{
	Node* node = pow();

	while (current_token.type == kw.mul || current_token.type == kw.div)
	{
		Token token = current_token;

		if (token.type == kw.mul)
			eat(kw.mul);
		if (token.type == kw.div)
			eat(kw.div);

		node = new BinOp(token,node, pow());
	}

	return node;
}

Node* Parser::expr()
{
	Node* node = term();

	while (current_token.type == kw.add || current_token.type == kw.sub)
	{
		Token token = current_token;

		if (token.type == kw.add)
			eat(kw.add);
		if (token.type == kw.sub)
			eat(kw.sub);

		node = new BinOp(token, node, term());
	}

	return node;
}



Node* Parser::parse()
{
	return program();
}
