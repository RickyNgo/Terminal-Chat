#include <cstdlib>
#include <string>
#include <cstring>
#include <ctime>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
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

      boost::shared_ptr<Client> c = boost::make_shared<Client>(io_service, iterator);
      std::thread t([&io_service](){ io_service.run(); });
      //io_service.run();

      c->choose_alias();
      char write_buffer[512];
      while ( std::cin.getline( write_buffer, 511 )) {
        c->send( write_buffer );
      }
      c->close(); 
      t.join();
  }

  catch (std::exception& e){
    std::cerr << "Exception: " << e.what() << "\n";
  }
  
  return 0;
}
