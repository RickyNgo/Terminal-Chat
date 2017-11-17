#include <cstdlib>
#include <string>
#include <iostream>
#include <boost/algorithm/string.hpp>

#include "aux_functions.hpp"

#define MIN_ALIAS 5
#define MAX_ALIAS 15


//verifies that argument meets syntax structure
// right now, this is doing both alias and pw. could restructure for custom matching.
// set to between 5 and 15 characters, alphanumeric only
bool syntax_valid_alias(std::string u_string){
	if(u_string.length() > MAX_ALIAS | u_string.length() < MIN_ALIAS){return false;}

	for(int i = 0; i<u_string.length(); i++){
		if(!isalnum(u_string[i])){return false;}
	}

	return true;
}

//prompts for input and captures string until newline is entered
/*Added D to function name as it was clashing with the UI get_string() function*/ 
std::string Dget_input(){
	std::string user_input;

	std::cout << ">> ";
	std::getline(std::cin, user_input, '\n');

	return user_input; 
}

//clean input of leading and trailing whitespace
//returns 1 if string starts with '/' or 0 if not
int clean_input(std::string input){
	boost::algorithm::trim(input);

	if(input[0] == '/'){return 1;}
	else {return 0;}

}

//strips first word off string
std::string get_command(std::string input){
	std::string delim = " ";

	std::string token = input.substr(0, input.find(delim));

	return token;
}

//compares argument against list of commands
//returns command # if found or -1 if not found
int find_command(std::string command){
	std::string command_list[] = {"/exit", "/show", "/join", "/create", "/whisper", "/online", "/help", "/invite", "/leave", "/kick", "/add_mod", "/channel_close"};

	boost::algorithm::to_lower(command);

	for(int i = 0; i < (sizeof(command_list)/sizeof(command_list[0])); i++){
		if(command == command_list[i]){
			return i;
		}
	}

	return -1;
}



// -1 if command is not allowed in current room for any level of permissions
// -2 if user does not have correct permissions for command in current room
int check_channel_permisisons(int command, Client* c){
	//get ChannelType of current channel
	ChannelType ct = (c->get_channel_from_id(c->get_current_channel_id()))->get_channel_type();
	ChannelRole cr = (c->get_channel_from_id(c->get_current_channel_id()))->get_channel_role();
 

 	if(ct == NO_TYPE){
 		return -1;
 	}

 	else if(ct == GROUP){
 		switch(cr){
			case GUEST:
				if(command > 8){
					return -2;
				}
				break;

			case MOD:
				if(command > 9){
					return -2;
				}
				break;

			case NO_ROLE:
				std::cout << "Error: you must have a role in a group" << std::endl;
				return -2;
				break;

			default:
				break;
		}
 	}

 	else if(ct == PRIVATE){
		if(command == 4 | command == 7 | command == 9 | command == 10 | command == 11){
			return -1;
		}
 	}
	
	return 0;
	
}


//returns 0 if general command, 1 if "in channel only", 2 if mod/admin only, 3 if admin only
int command_cat(int command){
	switch(command){
		case 0:
		case 2:
		case 3:
		case 4:
		case 5:
			return 0; //general
			break;
		case 7:
		case 8:
			return 1; //in channel only
			break;
		case 9:
			return 2; //mod or admin only
			break;
		case 10:
		case 11:
			return 3; //admin only
			break;
		default:
			return -1;
			break;
	}
}

//if command is a client side command, this function calls the appropriate action
void client_command(int command, std::string message){
	if(command == 1){ 
		//show channel
		std::cout << "Here is your channel" << std::endl;
	}
	else if(command == 6){
		std::cout << "here is some help" << std::endl;
	}
	else{
		std::cout << "error. not a client command" << std::endl;
	}
}

//if command is a server side command, checks permissions for desired command and either packages and sends message if allowed
//or displays error if not allowed
void server_command(int command, std::string message, Client* c){
	int perm = check_channel_permisisons(command, c);

	if(perm == -1){
		std::cout << "That command is not allowed here" << std::endl;
	}

	else if (perm == -2){
		std::cout << "You do not have the correct permissions for that action" << std::endl;
	}

	else{
		int command_type = command_cat(command);

		switch(command_type){
			case 0:
				//general command
				std::cout << "general command: making a Message and sending" << std::endl;
				//stick into Message and send
				break;
			case 1:
				//in channel only
				if(c->get_channel_from_id(c->get_current_channel_id())){
					std::cout << "sticking string in a Message and sending" << std::endl;
					//stick into Message and send
				}
				else{
					std::cout << "you need to be in a channel to do that" << std::endl;
				}
		}
	}



}