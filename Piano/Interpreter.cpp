#include "Interpreter.h"

unordered_map<string, Getter*> GLOBAL_GETTERS;

Getter::Getter(Set* set_)
{
	set = set_;
	value = get_value(set->base);
	terms.push_back(value);
	depth = 0;
}


void Interpreter::error()
{

}

string round_to(string var)
{
	string ret = "";
	size_t d = 0;

	for (size_t t = 0; t < var.size(); t++)
	{
		if (var[t] != '.' )
			ret += var[t];
		else
		{
			ret += '.';
			d = t+1;
			break;
		}
	}

	for (size_t t = d; t < d + 2 && t < var.size(); t++)
	{
		ret += var[t];
	}

    return ret;
}