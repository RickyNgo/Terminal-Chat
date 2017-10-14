#include "layout_new.hpp"
#include <unistd.h>
#include <signal.h>

WINDOW *chatWin, *chatWinBox, *channelWin, *channelWinBox, *inputWin, *inputWinBox, *contactWin, *contactWinBox;

int main()
{
    signal(SIGWINCH, resize_handler);

    win_init();

    while(1)
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