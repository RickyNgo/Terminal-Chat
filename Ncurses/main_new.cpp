#include "layout_new.hpp"
#include <unistd.h>
#include <signal.h>
#include <iostream>

WINDOW *chatWin, *chatWinBox, *channelWin, *channelWinBox, *inputWin, *inputWinBox, *contactWin, *contactWinBox, *loginWin, *loginWinBox;
int isRunning = 1;

std::vector<std::string> test = {"Ricky", "OBIWAN"};



int main()
{
    signal(SIGWINCH, resize_handler);

    win_init();
    splash_display();

    std::string alias = retrieve_alias();
    std::string input;

    while(isRunning)
    {
        //scroll(chatWinBox);
        wrefresh(chatWinBox);
        wrefresh(inputWinBox);
        wrefresh(channelWinBox);
        wrefresh(contactWinBox);
        input = get_input();
        display_chat();
        std::cout << "MOST RECENT INPUT: " << input << std::endl;
        //invite_notification(test);
        //sleep(1);
    }

    del_wins();

    std::cout << "ALIAS: " << alias << std::endl;
    std::cout << "MOST RECENT INPUT: " << input << std::endl;

    return 0;
}