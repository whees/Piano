#pragma once
#include "Node.h"
#include "Parser.h"
#include <stack>
#include <cmath>
#include <iomanip>
using namespace std;
class Seriaizer;
class Getter;
class Interpreter;


string round_to(string value);

extern unordered_map<string, Getter*> GLOBAL_GETTERS;

class Serializer : public Visitor
{
	string value;
public:

	static string get_value(Node* node)
	{
		Serializer ser;
		node->accept(&ser);

		return ser.value;
	}
	void return_(string value_)
	{
		value = value_;
	}

	void visit(Get* node) override
	{
		return_(node->token.value);
	}
	void visit(GetN* node) override
	{
		return_(node->token.value);
	}
	void visit(Succ* node) override
	{
		return_(node->token.value);
	}
	void visit(Self* node) override
	{
		return_(node->token.value);
	}
	void visit(Set* node) override
	{
		return_(node->token.value);
	}
	void visit(BinOp* node) override
	{
		return_(node->token.value);
	}
	void visit(UnOp* node) override
	{
		return_(node->token.value);
	}
	void visit(Num* node) override
	{
		return_(node->token.value);
	}
	void visit(Name* node) override
	{
		return_(node->token.value);
	}
	void visit(NoOp* node) override
	{
		return_(node->token.value);
	}
	void visit(Program* node) override
	{
		return_(node->token.value);
	}
	void visit(Define* node) override
	{
		return_(node->token.value);
	}
	void visit(Show* node) override
	{
		return_(node->token.value);
	}

};

class Getter : public Visitor
{
	vector<float> terms;
	stack<float> return_stack;
	Set* set;
	float value;
	size_t depth;
public:

	Getter(Set* set_);
	~Getter() {}
	float get(size_t depth_)
	{
		if (depth_ < terms.size())
			return terms[depth_];

		value = terms.back();
		depth = terms.size() - 1;
		while (depth < depth_)
		{
			value = get_value(set->succ);
			terms.push_back(value);
			depth++;
		}

		depth = 0;
		return value;
	}

	float get_value(Node* node)
	{
		node->accept(this);
		float value_ = return_stack.top();
		return_stack.pop();

		return value_;
	}
	void return_(float value_)
	{
		return_stack.push(value_);
	}

	void visit(Get* get_) override
	{
		string name_ = Serializer::get_value(get_->name);
		if (name_ != kw.self)
		{
			return_(GLOBAL_GETTERS[name_]->get(depth));
		}
		else
		{
			return_(value);

		}
	}
	void visit(GetN* getn_) override
	{
		string name_ = Serializer::get_value(getn_->name);
		size_t num = (size_t)get_value(getn_->num);

		if (name_ != kw.self)
		{
			return_(GLOBAL_GETTERS[name_]->get(num));

		}
		else
		{
			if (num == 0)
				return_(terms[0]);
			else
				return_(get(num));
		}
		
	}
	void visit(Succ* succ_) override
	{
		string name_ = Serializer::get_value(succ_->name);
		if (name_ != kw.self)
			return_(GLOBAL_GETTERS[name_]->get(depth+1));
		
		else
			return_(get_value(set->succ));
		
	}
	void visit(BinOp* binop_) override
	{
		Token token_ = binop_->token;

		if (token_.type == kw.add)
			return_(get_value(binop_->left) + get_value(binop_->right));
		else if (token_.type == kw.sub)
			return_(get_value(binop_->left) - get_value(binop_->right));
		else if (token_.type == kw.mul)
			return_(get_value(binop_->left) * get_value(binop_->right));
		else if (token_.type == kw.div)
			return_(get_value(binop_->left) / get_value(binop_->right));
		else if (token_.type == kw.pow)
			return_(pow(get_value(binop_->left), get_value(binop_->right)));
	}

	void visit(UnOp* unop_) override
	{
		Token token_ = unop_->token;

		if (token_.type == kw.add)
			return_(get_value(unop_->arg));
		else if (token_.type == kw.sub)
			return_(-get_value(unop_->arg));
	}
	
	void visit(Self* self) override {}
	void visit(Set* set) override {}
	void visit(Num* num_) override 
	{
		return_(num_->value);
	}
	void visit(Name* name) override {}
	void visit(NoOp* noop) override {}
	void visit(Program* prog) override {}
	void visit(Define* def) override {}
	void visit(Show* show) override {}

	void error() {}
};

class Interpreter : public Visitor
{
	Parser* parser;
	stack<float> return_stack;
public:
	Interpreter(Parser* parser_) : parser{ parser_ } {}
	Interpreter() { parser = new Parser; }
	~Interpreter() {}
	float get_value(Node* node)
	{
		node->accept(this);
		float value_ = return_stack.top();
		return_stack.pop();
		return value_;
	}
	void return_(float value_)
	{
		return_stack.push(value_);
	}

	int interpret() { parser->parse()->accept(this); return 0; }


	void visit(Get* get) override {}
	void visit(GetN* getn_) override
	{
		string name_ = Serializer::get_value(getn_->name);
		size_t num = (size_t)get_value(getn_->num);
		return_(GLOBAL_GETTERS[name_]->get(num));
	}
	void visit(Succ* succ) override {}
	void visit(Self* self) override {}
	void visit(Set* set) override {}
	void visit(BinOp* binop) override {}
	void visit(UnOp* unop) override {}
	void visit(Num* num_) override 
	{
		return_(num_->value);
	}
	void visit(Name* name) override {}
	void visit(NoOp* noop) override {}
	void visit(Program* prog) override 
	{
		for (Node* arg : prog->args)
		{
			arg->accept(this);
		}
	}
	void visit(Define* def_) override 
	{
		string name_ = Serializer::get_value(def_->name);
		GLOBAL_GETTERS[name_] = new Getter(def_->set);
	}
	void visit(Show* show) override 
	{
		cout << string_(show->arg) << endl;
	}

	string string_(Node* arg_)
	{
		Token token_ = arg_->token;
		if (token_.type == kw.getn)
			return to_string(get_value(arg_));
		if (token_.type == kw.name)
			return set_string(arg_);
	}

	string set_string(Node* name_)
	{
		string name__ = Serializer::get_value(name_);
		string out_string = "{";
		out_string += num_string(GLOBAL_GETTERS[name__]->get(0)) + ",";

		for (size_t i = 1; i < 4; i++)
		{
			out_string += " " + num_string(GLOBAL_GETTERS[name__]->get(i)) + ",";
		}

		out_string += "...}";
		return out_string;
	}
	
	string num_string(float num_)
	{
		if (floor(num_) - num_ == 0)
			return to_string(int(num_));
		return round_to(to_string(num_));
	}
	void error();
};

