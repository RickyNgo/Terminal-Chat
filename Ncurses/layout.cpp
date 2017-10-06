#include <unistd.h>
#include <string>
#include <sstream>
#include <panel.h>
#include <ctime>
#include <cstring>
#include "layout.hpp"


int main()
{
    int parent_x;
    int parent_y;
    int new_x;
	int new_y;

    int channel_width = 30;
    int contact_width = 30;
    int chat_box_height = 5;

    initscr();
    start_color();
    cbreak();
    getmaxyx(stdscr, parent_y, parent_x);

    WINDOW *channels = newwin(parent_y, channel_width, 0, 0);
    WINDOW *dialog = newwin(parent_y-chat_box_height, parent_x - channel_width - contact_width, 0, contact_width);
    WINDOW *contacts = newwin(parent_y, contact_width, 0, parent_x - contact_width);
    WINDOW *chat_box = newwin(chat_box_height, parent_x - channel_width - contact_width, parent_y - chat_box_height, channel_width);
    
    /*
    init_color(COLOR_RED, 245, 0, 0);
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_BLUE);


    wbkgd(channels, COLOR_PAIR(1) );
    wbkgd(contacts, COLOR_PAIR(2));
    wbkgd(chat_box, COLOR_PAIR(3));
    */

    box(dialog, 124, 45);
    box(chat_box, 124, 45);
    box(channels, 124, 45);
    box(contacts, 124, 45);

    while(1)
    {
        getmaxyx(stdscr, new_y, new_x);
		
		if (new_y != parent_y || new_x != parent_x)
		{
			parent_x = new_x;
			parent_y = new_y;

			wresize(dialog, parent_y-chat_box_height, parent_x - channel_width - contact_width);
			wresize(chat_box, chat_box_height, parent_x - channel_width - contact_width);
			
            mvwin(contacts, 0, parent_x - contact_width);
            mvwin(chat_box, parent_y - chat_box_height, channel_width);

			wclear(dialog);
			wclear(chat_box);
            wclear(contacts);
            wclear(channels);

            box(chat_box, 124, 45);
			box(dialog, 124, 45);
            box(channels, 124, 45);
            box(contacts, 124, 45);
            

		}

        mvwprintw(dialog, 1, 1, "DIALOG");
        mvwprintw(channels, 1, 1, "CHANNELS");
        mvwprintw(contacts, 1, 1, "CONTACTS");
        mvwprintw(chat_box, 1, 1, "CHATBOX");
		
        wrefresh(stdscr);
		wrefresh(contacts);
        wrefresh(dialog);
        wrefresh(channels);
        wrefresh(chat_box);
        refresh();

        sleep(1);
    }

    delwin(channels);
    delwin(dialog);
    delwin(contacts);
    delwin(chat_box);

    return 0;

}