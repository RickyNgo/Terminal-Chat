#include "layout.hpp"
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "client.hpp"

using boost::asio::ip::tcp;

WINDOW *chatWin, *chatWinBox, *channelWin, *channelWinBox, *inputWin, *inputWinBox, *contactWin, *contactWinBox, *loginWin, *loginWinBox;
int isRunning = 1;

std::vector<std::string> test = {"Ricky", "OBIWAN"};

int main(int argc, char* argv[])
{
    
//    immediately check command arguments
    if (argc != 2)
    {
        std::cerr << "Usage: <port>\n" << "argc == " << argc;
        return 1;
    }
    
    signal(SIGWINCH, resize_handler);

    win_init();
    splash_display();

    std::string alias = retrieve_alias();
    std::string input;
    
    boost::asio::io_service io_service;
    
    tcp::resolver resolver(io_service);
    tcp::resolver::query query("localhost", argv[1], tcp::resolver::query::canonical_name);
    tcp::resolver::iterator iterator = resolver.resolve(query);
    
    boost::shared_ptr<Client> c = boost::make_shared<Client>(io_service, iterator);
    std::thread t([&io_service](){ io_service.run(); });

    while(isRunning)
    {
        //scroll(chatWinBox);
        //wrefresh(chatWinBox);
        wrefresh(chatWin);
        //wrefresh(inputWinBox);
        wrefresh(inputWin);
        //wrefresh(channelWinBox);
        wrefresh(channelWin);
        wrefresh(contactWinBox);
//        input = get_input();
        display_chat();
        
        try{
            c->choose_alias();
            
            char write_buffer[512];
            while ( std::cin.getline( write_buffer, 511 )) {
                c->send( write_buffer );
            }
            c->close(); 
            t.join();
            
            isRunning = false; //just run once
        }
        
        catch (std::exception& e){
            std::cerr << "Exception: " << e.what() << "\n";
        }

        
        //std::cout << "MOST RECENT INPUT: " << input << std::endl;
        //invite_notification(test);
        //sleep(1);
    }

    del_wins();

    std::cout << "ALIAS: " << alias << std::endl;
    std::cout << COLORS << std::endl;
    return 0;
}