//#include "layout_new.hpp"
#include <ncurses.h>
#include <vector>
#include <string>
#include <ctime>
#include <cstring>
#include <stdlib.h>

// Windows to be used
extern WINDOW *chatWin, *chatWinBox, *channelWin, *channelWinBox, *inputWin, *inputWinBox, *contactWin, *contactWinBox;

// Controls the main while() loop
extern int isRunning;

// Global window parameters
int parent_x;
int parent_y;
int new_x;
int new_y;
int chatWinY = 3;

int channel_width = 30;
int contact_width = 30;
int chat_box_height = 7;

int channel_list_yindex = 2;
int dialog_list_yindex = 2;
int contact_list_yindex = 2;

char *buffer = new char[280];

std::vector<std::string> time_buffer;
std::vector<std::string> alias_buffer;
std::vector<std::string> chat_buffer;


void draw_channels()
{
    channelWinBox = newwin(parent_y, channel_width, 0, 0);
    box(channelWinBox, 124, 45);
    mvwprintw(channelWinBox, 1, 1, "Channels");
    scrollok(channelWinBox, TRUE);
    wrefresh(channelWinBox);
}

void draw_chat()
{
    chatWinBox = newwin(parent_y-chat_box_height, parent_x - channel_width - contact_width, 0, contact_width);
    box(chatWinBox, 124, 45);
    mvwprintw(chatWinBox, 1, 1, "ENKI");
    scrollok(chatWinBox, TRUE);
    wrefresh(chatWinBox);
}

void draw_contacts()
{
    contactWinBox = newwin(parent_y, contact_width, 0, parent_x - contact_width);
    box(contactWinBox, 124, 45);
    mvwprintw(contactWinBox, 1, 1, "Contacts");
    scrollok(contactWinBox, TRUE);
    wrefresh(contactWinBox);
}

void draw_input()
{
    inputWinBox = newwin(chat_box_height, parent_x - channel_width - contact_width, parent_y - chat_box_height, channel_width);
    box(inputWinBox, 124, 45);
    mvwprintw(inputWinBox, 1, 1, ">:");
    scrollok(inputWinBox, TRUE);
    wrefresh(inputWinBox);
}

int win_init()
{
    initscr();
    cbreak();
    getmaxyx(stdscr, parent_y, parent_x);

    draw_channels();
    draw_chat();
    draw_contacts();
    draw_input();
}

void del_wins()
{
    delwin(channelWinBox);
    delwin(channelWin);
    delwin(chatWinBox);
    delwin(chatWin);
    delwin(contactWinBox);
    delwin(contactWin);
    delwin(inputWinBox);
    delwin(inputWin);

    endwin();
}

void resize_handler(int sig)
{
    endwin();
    refresh();
    clear();

    getmaxyx(stdscr, new_y, new_x);

    if (new_y != parent_y || new_x != parent_x)
    {
        parent_x = new_x;
        parent_y = new_y;
    }

    draw_channels();
    draw_contacts();
    draw_chat();
    draw_input();

    /*
    mvwprintw(channelWinBox, 5, 1, "TEST");
    mvwprintw(chatWinBox, 5, 1, "TEST");
    mvwprintw(contactWinBox, 5, 1, "TEST");
    mvwprintw(inputWinBox, 5, 1, "TEST");
    */
    int window_limit = parent_y - chat_box_height - 2;
    int chat_buffer_pos = chat_buffer.size()-1;

    for (int i = chat_buffer.size(); i > 0; i--)
    {
        mvwprintw(chatWinBox, window_limit, 5, chat_buffer[chat_buffer_pos].c_str());
        window_limit--;
        chat_buffer_pos--;
        if (window_limit == 1)
        {
            break;
        }
    }
    
    box(chatWinBox, 124, 45);
    wmove(inputWinBox, 1, 4);
    wclear(inputWinBox);
    box(inputWinBox, 124, 45);
    mvwprintw(inputWinBox, 1, 1, ">:");
    mvwprintw(chatWinBox, 1, 1, "Enki");

    wcursyncup(inputWinBox);
    wrefresh(channelWinBox);
    wrefresh(chatWinBox);
    wrefresh(contactWinBox);
    wrefresh(inputWinBox);
}

void get_input()
{
    struct tm *time_info;
    time_t ts;

    memset(buffer, 0, 280);

    wmove(inputWinBox, 1, 4);
    wrefresh(inputWinBox);
    
    wgetnstr(inputWinBox, buffer, 280);
    if (buffer[0] == '\0')
    {
        return;
    }

    if (strcmp(buffer, "exit") == 0)
    {
        isRunning = 0;

        return;
    }

    time(&ts);

    time_info = localtime(&ts);
    char *haha = new char[12];

    sprintf(haha, "%02d:%02d:%02d", time_info->tm_hour, time_info->tm_min, time_info->tm_sec);

    std::string fmtd_time(haha);
    fmtd_time.append("| ");
    time_buffer.push_back(fmtd_time);

    std::string conv(buffer);
    chat_buffer.push_back(conv);
    
    alias_buffer.push_back("Ricky");
    /*
    wclear(chatWinBox);
    
    // Used to test user input being displayed. Will convert to separate function for handling messages received from the server
    int window_limit = parent_y - chat_box_height - 2;
    int chat_buffer_pos = chat_buffer.size()-1;

    for (int i = chat_buffer.size(); i > 0; i--)
    {
        mvwprintw(chatWinBox, window_limit, 1, time_buffer[chat_buffer_pos].c_str());
        mvwprintw(chatWinBox, window_limit, 12, chat_buffer[chat_buffer_pos].c_str());
        window_limit--;
        chat_buffer_pos--;
        if (window_limit == 1)
        {
            break;
        }
    }
    
    */

    box(chatWinBox, 124, 45);
    wmove(inputWinBox, 1, 4);
    wclear(inputWinBox);
    box(inputWinBox, 124, 45);
    mvwprintw(inputWinBox, 1, 1, ">:");
    //mvwprintw(chatWinBox, 1, 1, "Enki");
    wrefresh(inputWinBox);
    //wrefresh(chatWinBox);
}

void display_chat()
{
    wclear(chatWinBox);
    int window_limit = parent_y - chat_box_height - 2;
    int chat_buffer_pos = chat_buffer.size()-1;

    for (int i = chat_buffer.size(); i > 0; i--)
    {
        mvwprintw(chatWinBox, window_limit, 1, time_buffer[chat_buffer_pos].c_str());
        mvwprintw(chatWinBox, window_limit, 10, "<%s>", alias_buffer[chat_buffer_pos].c_str());
        mvwprintw(chatWinBox, window_limit, 20, ":%s", chat_buffer[chat_buffer_pos].c_str());
        window_limit--;
        chat_buffer_pos--;
        if (window_limit == 1)
        {
            break;
        }
    }

    box(chatWinBox, 124, 45);
    mvwprintw(chatWinBox, 1, 1, "Enki");
    wrefresh(chatWinBox);
}

void splash_display()
{
    mvwprintw(chatWinBox, 10, parent_x/3.4+1, " _______ .__   __.  __  ___  __ ");
    mvwprintw(chatWinBox, 11, parent_x/3.4+1, "|   ____||  \\ |  | |  |/  / |  |");
    mvwprintw(chatWinBox, 12, parent_x/3.4+1, "|  |__   |   \\|  | |  '  /  |  |");
    mvwprintw(chatWinBox, 13, parent_x/3.4+1, "|   __|  |  . `  | |    <   |  |");
    mvwprintw(chatWinBox, 14, parent_x/3.4+1, "|  |____ |  |\\   | |  .  \\  |  |");
    mvwprintw(chatWinBox, 15, parent_x/3.4+1, "|_______||__| \\__| |__|\\__\\ |__|");
}