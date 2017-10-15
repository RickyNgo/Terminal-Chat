#ifndef aux_functions
#define aux_functions

#include <cstdlib>
#include <string>

enum Commands {
	JOIN, 			//0
	EXIT, 			//1
	CREATE, 		//2
	WHISPER, 		//3
	INVITE, 		//4
	FRIEND_LIST, 	//5
	MOD_LIST, 		//6
	KICK_USER, 		//7
	KICK_MOD, 		//8
	BLOCK, 			//9
	ADD_FRIEND, 	//10
	ADD_CHANNEL, 	//11
	ADD_MOD,		//12
	MESSAGE_DELETE,	//13
	CHANNEL_CLOSE,	//14
	SHOW,			//15
	HELP,			//16
	ONLINE			//17
};

//verifies that argument meets syntax structure
//set to between 5 and 25 characters, alphanumeric only
bool syntax_valid_alias(std::string);

//prompts for input and captures string until newline is entered
void get_input();

//clean input of leading and trailing whitespace/newline
//returns 1 if string starts with '/' or 0 if not
int clean_input(std::string*);

//strips first word off string
std::string get_command(std::string);

//compares argument against list of commands
//returns command # if found or -1 if not
int find_command(std::string);





#endif