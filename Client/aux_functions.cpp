#include <cstdlib>
#include <string>
#include <iostream>

#include "aux_functions.hpp"


//verifies that argument meets syntax structure
// right now, this is doing both alias and pw. could restructure for custom matching.
// set to between 5 and 25 characters, alphanumeric only
bool syntax_valid_alias(std::string u_string){
	if(u_string.length() > 25 | u_string.length() < 5){return false;}

	for(int i = 0; i<u_string.length(); i++){
		if(!isalnum(u_string[i])){return false;}
	}

	return true;
}