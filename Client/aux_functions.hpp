#ifndef aux_functions
#define aux_functions

#include <cstdlib>
#include <string>

#include "client.hpp"
#include "channel.hpp"

enum ClientCommands {
	//general
	C_EXIT = 0, 
	C_SHOW = 1, 
	C_JOIN = 2, 
	C_CREATE = 3,
	C_WHISPER = 4, 	
	C_ONLINE = 5,
	C_HELP = 6, 

	//in channel only
	C_INVITE = 7, 
	C_LEAVE = 8,

	//channel mod or admin
	C_KICK = 9,

	//channel admin only
	C_ADD_MOD = 10,  
	C_CHANNEL_CLOSE = 11, 	
	};
//
////verifies that argument meets syntax structure
////set to between 5 and 25 characters, alphanumeric only
//bool syntax_valid_alias(std::string);

//prompts for input and captures string until newline is entered
std::string Dget_input();

//clean input of leading and trailing whitespace
//returns 1 if string starts with '/' or 0 if not
//int clean_input(std::string*);
//
////strips first word off string
//std::string get_command(std::string);
//
////compares argument against list of commands
////returns command # if found or -1 if not found
//int find_command(std::string);
//
//// -2 if command is not allowed in current room
//// -3 if user does not have correct permissions for command
//int check_channel_permisisons(int, Client*);
//
//// 0 if general command, 1 if channel only, 2 if mod/admin only, 3 if admin only
//int command_cat(int);
//
////if command is a client side command, this function calls the appropriate action
//void client_command(int, std::string);
//
////if command is a server side command, checks permissions for desired command and either packages and sends message if allowed
////or displays error if not allowed
//void server_command(int, std::string, Client*);





#endif