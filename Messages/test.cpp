#include "messages.hpp"
#include <iostream>

int main()
{
	time_t t = time(NULL);
	Messages test("Ricky", "LOves to program", t, MSG);

	char msg[] = "10|3000000|Ricky|I HOPE THIS WORKS";

//	Messages test2(msg);

	std::string test1 = test.get_header();
	std::string test2 = test.get_body();

	std::cout << test1 << std::endl;
	std::cout << test2 << std::endl;
	
	test.get_header() = "012801234567890RICKY0000000000";
	test.parse_header();

	std::cout << test.get_header() << std::endl;
	std::cout << test.get_command() << std::endl;
	std::cout << test.get_length() << std::endl;
	std::cout << test.get_time() << std::endl;
	std::cout << test.get_sender() << std::endl;

	/*
	std::cout << test2.get_sender() << std::endl;
	std::cout << test2.get_body() << std::endl;
	std::cout << test2.get_time() << std::endl;
	std::cout << test2.get_command() << std::endl;
	std::cout << test2.encode() << std::endl;
	*/

	return 0;
}
