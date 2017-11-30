#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <fstream>

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
        if (token == "/join") {
            user_cmd = JOIN;
            input = input.substr( token.length(), std::string::npos );
        } else {
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
        std::cerr << "Usage: <Command-Port> <Session-Port>\n" << "argc == " << argc;
        return 1;
    }

    signal(SIGWINCH, resize_handler);

    boost::asio::io_service io_service;
    
    tcp::resolver resolver(io_service);
    tcp::resolver::query query("localhost", argv[ 1 ], tcp::resolver::query::canonical_name);
    tcp::resolver::iterator iterator = resolver.resolve(query);
    
    boost::shared_ptr<Client> c = boost::make_shared<Client>(io_service, iterator, atoi(argv[2]));
    std::thread t([&io_service](){ io_service.run(); });

    win_init();
    //splash_display();

    std::string alias = retrieve_alias();
    std::string input = "";

    time_t current_time;
    Messages msg(alias, alias, time(&current_time), LOGIN);
    c->send(msg);

    std::ofstream log;
    log.open("log.txt", std::fstream::out | std::fstream::app);
    log << alias << std::endl;
    log << input << std::endl; 
    log << current_time << std::endl;
    log << msg.get_command() << std::endl;

    while(isRunning)
    {
        wrefresh(chatWin);
        wrefresh(inputWin);
        //wrefresh(channelWin);
        wrefresh(contactWinBox);
        
        //input = get_input();
        input = get_input();
        
        //Parse the input to determine what the command should be
        Commands cmd = get_command( input );

        if (cmd == JOIN )
        {
            std::string port( argv[ 2 ] );
            input += " " + port;
            Messages input_msg( alias, input, time( &current_time ), cmd );
            c->create_channel(input_msg.get_body());
            c->send(input_msg);
        } else if ( cmd == MSG ) {
            if (c->get_current_channel() != NULL)
            {
                Messages input_msg( alias, input, time( &current_time ), cmd );
                c->get_current_channel()->send(input_msg);
            }

        }            
    }
    c->close();
    del_wins();
    t.detach();

    return 0;
}