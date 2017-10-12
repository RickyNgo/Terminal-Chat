#ifndef helper_hpp
#define helper_hpp

#include <cstdlib>
#include <string>

#include "client.hpp"

void new_account(Client*); 
void login(Client*);

//asks server to verify if username is available
//uses client instance to send data
//returns true if yes, false if not
bool alias_available(Client*, std::string); 

//asks server if alias already exists
bool alias_exists(Client*, std::string);

//checks if password matches
int password_valid(Client*, std::string);

//passes name and password to server to set the password
void set_password(Client*, std::string, std::string); 

//verifies that string meets syntax structure
// <= 25 char, alphanumeric only
bool syntax_valid(std::string);

int validate_user_input(int, int, int);


#endif