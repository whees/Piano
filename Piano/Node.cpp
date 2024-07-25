#include "Node.h"

void Visitor::visit(Node* node) 
{ 
	cout << "visited " << node->token.value << endl;
	for (Node* n : node->nodes)
	{
		visit(n);
	}
};






