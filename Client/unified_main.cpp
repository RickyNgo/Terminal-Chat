#include "layout.hpp"
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "client.hpp"

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

    boost::asio::io_service io_service;
    
    tcp::resolver resolver(io_service);
    tcp::resolver::query query("localhost", argv[1], tcp::resolver::query::canonical_name);
    tcp::resolver::iterator iterator = resolver.resolve(query);
    
    boost::shared_ptr<Client> c = boost::make_shared<Client>(io_service, iterator);
    std::thread t([&io_service](){ io_service.run(); });

    std::string alias = retrieve_alias();
    std::string input = "";

    time_t current_time;
    time(&current_time);
    Messages msg(alias, input, current_time, LOGIN);
    c->send(msg);

    while(isRunning)
    {
        wrefresh(chatWin);
        wrefresh(inputWin);
        wrefresh(channelWin);
        wrefresh(contactWinBox);

        for (int i = 0; i < 30; i ++)
        {
            display_chat();
        }
        
        //input = get_input();
        input = get_input();
        Messages input_msg(alias, input, time(&current_time), MSG);
        c->send(input_msg);
        //display_chat();
        
        for (int i = 0; i < 30; i ++)
        {
            display_chat();
        }
    
    }
    c->close();
    del_wins();

    return 0;
}