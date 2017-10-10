//#include <unistd.h>
#include "layout.hpp"
#include <ctime>
#include <stdlib.h>

//#include "content_display.hpp"

void UI::draw_borders(WINDOW *screen) 
{
    int x, y, i;

    getmaxyx(screen, y, x);

    // sides
    for (i = 1; i < (y - 1); i++) {
        mvwprintw(screen, i, 0, "|");
        mvwprintw(screen, i, x - 1, "|");
    }

    for (i = 1; i < (x-1); i++)
    {
        mvwprintw(screen, y-1, i, "-");
    }
}

int UI::windows_init()
{
    initscr();
    start_color();
    cbreak();
    getmaxyx(stdscr, this->parent_y, this->parent_x);

    this->channels = newwin(parent_y, channel_width, 0, 0);
    this->dialog = newwin(parent_y-chat_box_height, parent_x - channel_width - contact_width, 0, contact_width);
    this->contacts = newwin(parent_y, contact_width, 0, parent_x - contact_width);
    this->chat_box = newwin(chat_box_height, parent_x - channel_width - contact_width, parent_y - chat_box_height, channel_width);
    
    box(this->dialog, 124, 45);
    box(this->chat_box, 124, 45);
    box(this->channels, 124, 45);
    box(this->contacts, 124, 45);
}

void UI::resize()
{
    getmaxyx(stdscr, this->new_y, this->new_x);
		
		if (new_y != parent_y || new_x != parent_x)
		{
			parent_x = new_x;
			parent_y = new_y;

			wresize(this->dialog, parent_y-chat_box_height, parent_x - channel_width - contact_width);
			wresize(this->chat_box, chat_box_height, parent_x - channel_width - contact_width);
			
            mvwin(this->contacts, 0, parent_x - contact_width);
            mvwin(this->chat_box, parent_y - chat_box_height, channel_width);

			wclear(this->dialog);
			wclear(this->chat_box);
            wclear(this->contacts);
            wclear(this->channels);

            box(this->chat_box, 124, 45);
			box(this->dialog, 124, 45);
            box(this->channels, 124, 45);
            box(this->contacts, 124, 45);
            

		}

        mvwprintw(this->dialog, 1, 1, "DIALOG");
        mvwprintw(this->channels, 1, 1, "CHANNELS");
        mvwprintw(this->contacts, 1, 1, "CONTACTS");
        mvwprintw(this->chat_box, 1, 1, "CHATBOX");
		
        wrefresh(stdscr);
		wrefresh(this->contacts);
        wrefresh(this->dialog);
        wrefresh(this->channels);
        wrefresh(this->chat_box);
        refresh();
}

void UI::delete_windows()
{
    delwin(this->channels);
    delwin(this->dialog);
    delwin(this->contacts);
    delwin(this->chat_box);
}


void UI::side_display(WINDOW *screen, std::vector<std::string> list)
{
    if (screen == this->channels)
    {
        for (int i = 0; i < list.size(); i++)
        {
            mvwprintw(this->channels, channel_list_yindex, 1, list[i].c_str());
            channel_list_yindex++;
        }
    }
    else if (screen == this->contacts)
    {
        for (int i = 0; i < list.size(); i++)
        {
            mvwprintw(this->contacts, contact_list_yindex, 1, list[i].c_str());
            contact_list_yindex++;
        }
    }
    else
    {
        return;
    }
}

void UI::chat_display(std::vector<std::string> list)
{
    for (int i = 0; i < list.size(); i++)
    {
        time_t rawtime;
        struct tm * timeinfo;
        time(&rawtime);
        timeinfo = gmtime(&rawtime);

        int hours = (timeinfo->tm_hour+(+8))%24;
        int minutes = timeinfo->tm_min;


        // Time
        mvwprintw(this->dialog, dialog_list_yindex, 1, std::to_string(hours).c_str());
        
        // User
        mvwprintw(this->dialog, dialog_list_yindex, 26, "Ricky");

        // Message
        mvwprintw(this->dialog, dialog_list_yindex, 35, "TESTING MY TEST");

        dialog_list_yindex++;
    }
}

void UI::splash_display()
{
    mvwprintw(this->dialog, 10, 60, " _______ .__   __.  __  ___  __ ");
    mvwprintw(this->dialog, 11, 60, "|   ____||  \\ |  | |  |/  / |  |");
    mvwprintw(this->dialog, 12, 60, "|  |__   |   \\|  | |  '  /  |  |");
    mvwprintw(this->dialog, 13, 60, "|   __|  |  . `  | |    <   |  |");
    mvwprintw(this->dialog, 14, 60, "|  |____ |  |\\   | |  .  \\  |  |");
    mvwprintw(this->dialog, 15, 60, "|_______||__| \\__| |__|\\__\\ |__|");
}

std::vector<std::string> list = {"one", "two", "three", "four"};

void UI::content_update()
{
    this->channel_list_yindex = 2;
    this->dialog_list_yindex = 2;
    this->contact_list_yindex = 2;

    side_display(this->channels, list);
    side_display(this->contacts, list);
    chat_display(list);
}