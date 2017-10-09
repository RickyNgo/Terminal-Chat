#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include "client.hpp"
#include "helper.hpp"

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: chat_client <host> <port>\n";
      return 1;
    }

    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    auto endpoint_iterator = resolver.resolve({ argv[1], argv[2] });
    Client c(io_service, endpoint_iterator);

    int menu_input;
    std::cout << "1.Login" << '\n' << "2.Create new account" <<std::endl;
    std::cin >> menu_input;

    int choice = validate_user_input(menu_input, 1, 2); 

    // login
    if(choice == 1){login(&c);}

    //new account
    else{new_account(&c);}

    //START CHATTING


    //below is placeholder from example
    /*
    std::thread t([&io_service](){ io_service.run(); });

    char line[chat_message::max_body_length + 1];
    while (std::cin.getline(line, chat_message::max_body_length + 1))
    {
      chat_message msg;
      msg.body_length(std::strlen(line));
      std::memcpy(msg.body(), line, msg.body_length());
      msg.encode_header();
      c.write(msg);
    }
*/
    c.close();
    //t.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  
  return 0;
}