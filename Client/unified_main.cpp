#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "client.hpp"
#include "../Ncurses/layout.hpp"

WINDOW *chatWin, *chatWinBox, *channelWin, *channelWinBox, *inputWin, *inputWinBox, *contactWin, *contactWinBox, *loginWin, *loginWinBox;
int isRunning = 1;

std::vector<std::string> test = {"Ricky", "OBIWAN"};

/*****************/
Commands get_command(std::string &input)
{
    Commands user_cmd = MSG;
    std::string token;
    if (input[0] == '/')
    {
        token = input.substr(0, input.find(" "));

        if (token == "/create_channel")
        {
            user_cmd = CREATE_CHANNEL;
            input = input.substr(token.length(), std::string::npos);
        }
        else if (token == "/join_channel")
        {
            user_cmd = JOIN_CHANNEL;
            input = input.substr(token.length(), std::string::npos);
        }
        else 
        {
            user_cmd = MSG;
        }
    }

    return user_cmd;
}

/****************/


int main(int argc, char* argv[])
{
    //    immediately check command arguments
    if (argc != 3)
    {
        std::cerr << "Usage: <port> <port>\n" << "argc == " << argc;
        return 1;
    }

    signal(SIGWINCH, resize_handler);

    boost::asio::io_service io_service;
    
    tcp::resolver resolver(io_service);
    tcp::resolver::query query("localhost", argv[1], tcp::resolver::query::canonical_name);
    tcp::resolver::iterator iterator = resolver.resolve(query);
    
    boost::shared_ptr<Client> c = boost::make_shared<Client>(io_service, iterator, atoi(argv[2]));
    std::thread t([&io_service](){ io_service.run(); });

    win_init();
    splash_display();

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
        
        //input = get_input();
        input = get_input();
        
        //Parse the input to determine what the command should be
        Commands cmd = get_command(input);
        
        Messages input_msg(alias, input, time(&current_time), cmd);
        if (cmd == CREATE_CHANNEL)
        {
            c->create_channel(input_msg.get_body());
        }

        c->send(input_msg);
    
    }
    c->close();
    del_wins();
    t.detach();

    return 0;
}