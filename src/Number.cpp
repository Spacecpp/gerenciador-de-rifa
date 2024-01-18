#include "Number.h"

Number::Number(): taken(false), value(0)
{
	
}


Number::Number(int value): taken(false), value(value)
{
	
	
}


Number::Number(int value, const std::string & name): taken(true), value(value), name(name)
{
	
}



void Number::set(const std::string & name)
{
	
	this->name = name;
	this->taken = true;
	
}


void Number::unset()
{
	
	this->name.clear();
	this->taken = false;
	
}