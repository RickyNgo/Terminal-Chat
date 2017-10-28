#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <ctime>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "boost/array.hpp"

#include "client.hpp"
#include "aux_functions.hpp"
#include "messages.hpp"

using boost::asio::ip::tcp;

enum { max_length = 1024 };

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: <port>\n" << "argc == " << argc;
      return 1;
    }

	boost::asio::io_service io_service;

  tcp::resolver resolver(io_service);
  tcp::resolver::query query("localhost", argv[1], tcp::resolver::query::canonical_name);
  tcp::resolver::iterator iterator = resolver.resolve(query);

  Client c(io_service, iterator);

	int valid_name = 0, command_num = -1;
	std::string alias, alias_req, u_input, command;
  char ret_msg[512];
	size_t request_length;
  time_t current_time;

	std::thread t([&io_service](){ io_service.run(); });


	//Get alias, verify syntax is correct, send to server for confirmation of uniqueness
	do
	  {
	  	std::cout << "Choose an alias: ";

	  	std::getline(std::cin, alias);

	  	valid_name = syntax_valid_alias(alias);

	  	if(!valid_name){
	  		std::cout << '\n' << "Not a valid alias. Must be between 5 and 15 alphanumeric characters" << std::endl;
	  	}

	  	else{
        //encode input
        time(&current_time);
        Messages alias_construct("", alias, current_time, LOGIN);
        valid_name = true;
	  		
        // send to server
    		boost::asio::write(*(c.get_main_socket()), boost::asio::buffer(alias_req, alias_req.length()));
            

	  		// get response
        boost::asio::read(*(c.get_main_socket()), boost::asio::buffer(ret_msg, 512));


	  		// decode with Message
        Messages alias_decode(ret_msg);

	  		// update valid_name to true depending on response
        if(alias_decode.get_body() == "OK"){
          c.set_user_alias(alias);
          std::cout << "Success! Here is the message body: " << alias_decode.get_body() << std::endl;
          valid_name = true;
        }
        else{
          std::cout << "Alias is taken. Choose another." << std::endl;
        }

	  	}
	  } while (!valid_name); 

    // while(1){
    //   u_input = get_input(); //replace with ricky's function
    //   command = get_command(u_input);

    //   command_num = find_command(command);

    //   if(command_num == 1 | command_num == 6){
    //     client_command(command_num, u_input);
    //   }
   
    //   else{
    //     server_command(command_num, u_input, &c);
    //   } 
    // } 

    c.close(); 
    t.join();
  }

  catch (std::exception& e){
    std::cerr << "Exception: " << e.what() << "\n";
  }
  
  return 0;
}
