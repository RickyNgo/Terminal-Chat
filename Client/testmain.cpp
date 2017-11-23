#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "boost/array.hpp"

#include "client.hpp"
#include "channel.hpp"
#include "aux_functions.hpp"

std::string ChannelType_to_string(ChannelType ct);
std::string ChannelRole_to_string(ChannelRole cr);

int main(int argc, char* argv[])
{
	try{
	    if (argc != 3){
	      std::cerr << "Usage: <port> <port>\n" << "argc == " << argc;
	      return 1;
		}

		std::string u_input;
		
		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		tcp::resolver::query query("localhost", argv[1], tcp::resolver::query::canonical_name);
		tcp::resolver::iterator iterator = resolver.resolve(query);

		Client c(io_service, iterator, atoi(argv[2]));

		// //make a channel object
		// Channel* testchannel = new Channel("testchannel", 1);
		// //add it to the client using add_channel
		// c.add_channel(testchannel, 1);
		// //set client's current channel to new channel
		// c.set_current_channel(testchannel);
		// //set testchannel type to group
		// testchannel->set_channel_type(GROUP);
		// testchannel->set_channel_role(ADMIN); 

		// while(u_input != "rachel"){
		// u_input = Dget_input();
		// int hasCommand = c.clean_input(u_input);

		c.create_channel("name");
		std::cout << "Channel name: " << c.get_current_channel()->get_channel_name() << std::endl;

		// ChannelRole cr = (c.get_channel_from_id(c.get_current_channel_id()))->get_channel_role();		
		// std::cout << "channel role is: " << ChannelRole_to_string(cr) << std::endl;

		

	}  //try
    
    catch (std::exception& e){
    	std::cerr << "Exception: " << e.what() << "\n";
  	}

  return 0;
}

std::string ChannelType_to_string(ChannelType ct){
	switch(ct){
		case PRIVATE: return "private";
		case GROUP: return "group";
		case NO_TYPE: return "no type";
		default: return "not a channel type";
	}
}

std::string ChannelRole_to_string(ChannelRole cr){
		switch(cr){
		case ADMIN: return "admin";
		case MOD: return "mod";
		case GUEST: return "guest";
		case OWNER: return "owner";
		case NO_ROLE: return "no role";
		default: return "not a channel role";
	}
}
