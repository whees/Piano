#pragma once
#include "Token.h"
#include <iostream>
#include <vector>
#define VISITABLE void accept(Visitor* visitor) override { return visitor->visit(this); };

using namespace std;

class Get;
class GetN;
class Set;
class Num;
class Name;
class NoOp;
class Program;
class Define;
class Show;
class Succ;
class BinOp;
class UnOp;
class Self;

class Visitor
{
public:
	virtual void visit(Get* get) = 0;
	virtual void visit(Set* set) = 0;
	virtual void visit(Num* num) = 0;
	virtual void visit(Name* name) = 0;
	virtual void visit(NoOp* noop) = 0;
	virtual void visit(Program* prog) = 0;
	virtual void visit(Define* def) = 0;
	virtual void visit(Show* show) = 0;
	virtual void visit(Succ* succ) = 0;
	virtual void visit(BinOp* binop) = 0;
	virtual void visit(UnOp* unop) = 0;
	virtual void visit(GetN* getn) = 0;
	virtual void visit(Self* self) = 0;
};

class Node
{
public:
	Token token;

	Node(Token token_) : token{ token_ } {}
	Node() : token{ Token(kw.null,kw.null) } {}
	~Node() {}

	virtual void accept(Visitor* visitor) = 0;
};

class NoOp : public Node 
{
public:
	NoOp() : Node() {};
	VISITABLE
};

class Program : public Node
{
public:
	Program() : Node(Token(kw.program,kw.program)) {}
	vector<Node*> args;
	void append(Node* node) { args.push_back(node); };
	VISITABLE
};

class Num : public Node
{
public:
	Num(Token token_) : Node(token_), value{ (float)stoi(token_.value) } {}
	float value;
	VISITABLE
};

class Name : public Node
{
public:
	Name(Token token_) : Node(token_), value{ token_.value } {}
	string value;
	VISITABLE
};

class Set : public Node
{
public:
	Set(Node* base_, Node* succ_) : Node(Token(kw.set, kw.set)), base{ base_ }, succ{ succ_ } {}
	Node* base;
	Node* succ;

	VISITABLE
};

class BinOp : public Node
{
public:
	BinOp(Token token_, Node* left_, Node* right_) : Node(token_), left{ left_ }, right{ right_ } {}
	Node* left;
	Node* right;
	VISITABLE
};

class UnOp : public Node
{
public:
	UnOp(Token token_, Node* arg_) : Node(token_), arg{ arg_ } {}
	Node* arg;
	VISITABLE
};

class Define : public Node
{
public:
	Define(Node* name_, Set* set_) : Node(Token(kw.def, kw.def)), name{ name_ }, set{ set_ } {}
	Node* name;
	Set* set;
	VISITABLE
};

class Show : public Node
{
public:
	Show(Node* arg_) : Node(Token(kw.show, kw.show)), arg{ arg_ } {};
	Node* arg;
	VISITABLE
};

class Get : public Node
{
public:
	Get(Node* name_) : Node(Token(kw.get, kw.get)), name{ name_ } {}
	Node* name;
	VISITABLE
};

class GetN : public Node
{
public:
	GetN(Node* name_, Node* num_) : Node(Token(kw.getn, kw.getn)), name{ name_ }, num{ num_ } {}
	Node* name;
	Node* num;
	VISITABLE
};

class Succ : public Node
{
public:
	Succ(Node* name_) : Node(Token(kw.show, kw.show)), name{ name_ } {}
	Node* name;
	VISITABLE
};

class Self : public Node
{
public:
	Self() : Node(Token(kw.self, kw.self)) {}
	VISITABLE
};

Node* make_Num(string value);

Node* make_Name(string value);





