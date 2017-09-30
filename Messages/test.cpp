#include "messages.hpp"
#include <iostream>

int main()
{
	Messages test1;
	Messages test2("Test2", "This is a test for test2");
	Messages test3("Test3", "Test3", "This is a test for test3");
	
	Messages tests[3] = {test1, test2, test3};
		
	for (int i = 0; i < 3; i++)
	{
		std::cout << tests[i].length() << std::endl;
		std::cout << tests[i].body() << std::endl;
	}

	return 0;
}
