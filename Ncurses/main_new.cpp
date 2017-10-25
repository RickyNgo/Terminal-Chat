#include "layout_new.hpp"
#include <unistd.h>
#include <signal.h>

WINDOW *chatWin, *chatWinBox, *channelWin, *channelWinBox, *inputWin, *inputWinBox, *contactWin, *contactWinBox;
int isRunning = 1;

int main()
{
    signal(SIGWINCH, resize_handler);

    win_init();
    splash_display();

    while(isRunning)
    {
        //scroll(chatWinBox);
        wrefresh(chatWinBox);
        wrefresh(inputWinBox);
        wrefresh(channelWinBox);
        wrefresh(contactWinBox);
        get_input();
        display_chat();
        //sleep(1);
    }

    del_wins();
}