#include <cstdlib>
#include <string>
#include <cstring>
#include <ctime>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include "client.hpp"

using boost::asio::ip::tcp;


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
      
      std::thread t([&io_service](){ io_service.run(); });

      Client c(io_service, iterator);
      
      c.choose_alias();


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
