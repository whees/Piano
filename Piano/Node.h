#pragma once
#include "Token.h"
#include <iostream>
#include <vector>
using namespace std;

class Node;

class Visitor
{
public:
	void visit(Node* node);
};

class Node
{
public:
	Token token;
	vector<Node*> nodes;

	Node(Token t) { token = t; }
	Node() { token = Token(); }
	~Node() {}

	void append(Node* node) { nodes.push_back(node); };
};



