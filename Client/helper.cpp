#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "client.hpp"
#include "helper.hpp"

/*setting up a new account
need to add option to quit input loop
*/
void new_account(Client* user){
	std::string name, pw;
	bool name_valid = false, pw_valid = false;

	// choose alias
	do{
		std::cout << "Choose an alias: " << std::endl;
		std::getline(std::cin, name);

		name_valid = syntax_valid(name);

		if(!name_valid){
			std::cout << "Invalid syntax. Must be 25 characters or less, alphanumeric only." << std::endl;
		}
        else{
			name_valid = alias_available(user, name);

			if(!name_valid){
			std::cout << "That alias is not available. Please choose another." << std::endl;
			}
        }

    }while(!name_valid);

	// set password
	do{
		std::cout << "Choose a password: " << std::endl;
		std::getline(std::cin, pw);

		pw_valid = syntax_valid(pw);

		if(!pw_valid){
			std::cout << "Invalid syntax. Must be 25 characters or less, alphanumeric only." << std::endl;
		}

		else{
			set_password(user, name, pw);
		}
	}while(!pw_valid);
} 

/*
login
*/
void login(Client* user){
	std::string name, pw;
	bool name_valid = false;
	int pw_valid = 0;

	do{
		std::cout << "Alias: " << std::endl;
		std::getline(std::cin, name);

		name_valid = syntax_valid(name);

		if(!name_valid){
			std::cout << "Not a valid alias." << std::endl;
		}

		else{
            name_valid = alias_exists(user, name);

			if(!name_valid){
				std::cout << "That alias does not exist." << std::endl;
			}

			else{
				do{
					std::cout << "Password: " << std::endl;
					std::getline(std::cin, pw);

					pw_valid = password_valid(user, pw);

					if(!pw_valid){
						std::cout << "Password incorrect." << std::endl;
					}
				}while(!pw_valid);

				user->setup_client(name, pw_valid);
            }
        }
	}while(!name_valid);
}

//asks server to verify if username is available
//uses client instance to send data
//returns true if yes, false if not
bool alias_available(Client*, std::string){
    return true;

}

//asks server if alias already exists
bool alias_exists(Client*, std::string){
    return true;
}

//asks server if password matches
//return user id if yes
int password_valid(Client*, std::string){
    return 1;
}

//passes name and password to server to set the password for alias
void set_password(Client*, std::string, std::string){

} 

//verifies that argument meets syntax structure
// right now, this is doing both alias and pw. could restructure for custom matching.
// set to between 5 and 25 characters, alphanumeric only
bool syntax_valid(std::string u_string){
	if(u_string.length() > 25 | u_string.length() < 5){return false;}

	for(int i = 0; i<u_string.length(); i++){
		if(!isalnum(u_string[i])){return false;}
	}

	return true;
}

//valids numeric input
int validate_user_input(int input, int start, int end)
{
    std::cin.ignore();
    
    while((input < start) | (input > end))
        {
            std::cout << "Sorry, not a valid choice. Choose again.\n";
            std::cin >> input;
            std::cin.ignore();
        }
    return input;
}
