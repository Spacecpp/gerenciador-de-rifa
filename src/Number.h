#ifndef NUMBER_H
#define NUMBER_H

#include <string>

class Number
{
	
	public:
		bool taken;
		int value;
		std::string name;
		
		Number();
		Number(int value);
		Number(int value, const std::string & name);
		
		void set(const std::string & name);
		void unset();
	
};

#endif
