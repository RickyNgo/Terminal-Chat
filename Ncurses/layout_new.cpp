#include "layout_new.hpp"
#include <ncurses.h>
#include <vector>
#include <string>
#include <ctime>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>

// Windows to be used
extern WINDOW *chatWin, *chatWinBox, *channelWin, *channelWinBox, *inputWin, *inputWinBox, *contactWin, *contactWinBox, *loginWin, *loginWinBox;

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
char *alias = new char[28];

/*
struct Buffer
{
    std::vector<std::string> time_buffer;
    std::vector<std::string> alias_buffer;
    std::vector<std::string> chat_buffer;
    std::string channel;
};*/

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
    //keypad(stdscr, TRUE);
    getmaxyx(stdscr, parent_y, parent_x);

    draw_login();
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

std::string get_input()
{
    struct tm *time_info;
    time_t ts;

    std::string for_client;

    memset(buffer, 0, 280);

    wmove(inputWinBox, 1, 4);
    wrefresh(inputWinBox);
    
    wgetnstr(inputWinBox, buffer, 280);

    if (buffer[0] == '\0')
    {
        return "";
    }
    else if (strcmp(buffer, "/exit") == 0)
    {
        isRunning = 0;

        return "";
    }
    else if (strcmp(buffer, "/help") == 0)
    {
        show_help();
        //return;
    }
    else
    {
        // The code below pretty much echos back what you type in
        time(&ts);

        time_info = localtime(&ts);
        char *raw_time = new char[12];

        sprintf(raw_time, "%02d:%02d:%02d", time_info->tm_hour, time_info->tm_min, time_info->tm_sec);

        std::string conv(buffer);
        chat_buffer.push_back(conv);

        std::string fmtd_time(raw_time);
        fmtd_time.append("| ");
        time_buffer.push_back(fmtd_time);

        
        alias_buffer.push_back(alias);

        // This will actually return the inputted string with the time appended to the front
        for_client.append(raw_time);

        for_client.append("|");
        for_client.append(conv);
    }
    
    /////////////////////////////////////////////////////////
    // Keep this section as is to refresh the input window
    box(chatWinBox, 124, 45);
    wmove(inputWinBox, 1, 4);
    wclear(inputWinBox);
    box(inputWinBox, 124, 45);
    mvwprintw(inputWinBox, 1, 1, ">:");
    wrefresh(inputWinBox);

    return for_client;
}

void display_chat()
{
    // I dont think this will need to be modified. The Client would put the contents of the Message received
    // from the server into the buffers.
    wclear(chatWinBox);
    int window_limit = parent_y - chat_box_height - 2;
    int chat_buffer_pos = chat_buffer.size()-1;

    for (int i = chat_buffer.size(); i > 0; i--)
    {
        mvwprintw(chatWinBox, window_limit, 1, time_buffer[chat_buffer_pos].c_str());
        mvwprintw(chatWinBox, window_limit, 10, "<%s>", alias_buffer[chat_buffer_pos].c_str());
        mvwprintw(chatWinBox, window_limit, 26, ":%s", chat_buffer[chat_buffer_pos].c_str());
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

void show_help()
{
    int y;
    int x;

    getmaxyx(chatWinBox, y, x);
    WINDOW *helpWin = subwin(chatWinBox, 23, 90, y/3, x/3);
    //WINDOW *helpWin = subwin(chatWinBox, 25, 75, LINES/3, COLS/3);

    
    keypad(helpWin, TRUE);
    

    std::string help = "\n"
                        " Commands are:\n"
                        " /help - Displays help commands and syntax\n"
                        " /exit - Exits the Enki chat program\n"
                        " /join [room] - Joins the chosen channel if available\n"
                        " /create [room] - Creates the chosen channel\n"
                        " /whisper [user] - Sends a direct message to another user\n"
                        " /invite [user] - Invites another user to your current channel\n"
                        " /mods - Displays the moderators for the current channel\n"
                        " /kick [user] - (MODS/ADMINS ONLY) Kicks the selected user from the channel\n"
                        " /add_mod [user] - (ADMINS ONLY) Adds the selected user to the channel's mod team\n"
                        " /channel_close - (ADMINS ONLY) Closes the current channel\n"
                        " /online [user] - See if the user is currently online\n"
                        " /show [room] - Displays all the rooms you are connected to\n"
                        " PRESS ANY KEY TO CLOSE THIS WINDOW";

    mvwprintw(helpWin, 0, 0, help.c_str());
    box(helpWin, 124, 45);

    wrefresh(helpWin);
    
    while(!wgetch(helpWin))
    {
        return;

    }

    delwin(helpWin);
}

void show_friends(std::vector<std::string> friends)
{
    for (int i = 0; i < friends.size(); i++)
    {
        mvwprintw(contactWinBox, 1, 2, friends[i].c_str());
    }
}

void show_channels(std::vector<std::string> channels)
{
    for (int i = 0; i < channels.size(); i++)
    {
        mvwprintw(channelWinBox, 1, 2, channels[i].c_str());
    }
}

int invite_notification(std::vector<std::string> inviter)
{
    WINDOW *popup = newwin(20, 20, parent_y - 40, 0);

    box(popup, 124, 45);

    std::string alert = inviter[0] + " invites you to " + inviter[1];
    mvwprintw(popup, 1, 1, alert.c_str());

    std::string response = "Press (y) to accept or (n) to decline";

    mvwprintw(popup, 10, 1, response.c_str());
    wrefresh(popup);
    sleep(1);

    
    delwin(popup);
    
}

void get_alias()
{
    while(1)
    {
        memset(alias, 0, 28);

        wmove(loginWinBox, parent_y/2+3, parent_x/2-17);
        wrefresh(loginWinBox);


        wgetnstr(loginWinBox, alias, 15);
        
        std::string temp(alias);

        int count = 0;
        for (int i = 0; i < temp.length(); i++)
        {
            if (temp[i] == ' ')
            {
                break;
            }
            else 
            {
                count++;
            }

        }       
        
        if (count == temp.length())
        {
            return;
        }
    }
    

    
}

void draw_login()
{
    loginWinBox = newwin(0, 0, 0, 0);
    box(loginWinBox, 124, 45);

    mvwprintw(loginWinBox, parent_y/2, parent_x/2-18, "LOGIN" );
    mvwprintw(loginWinBox, parent_y/2+1, parent_x/2-18, "Please enter an alias 5-15 characters long");
    mvwprintw(loginWinBox, parent_y/2+3, parent_x/2-18, ": ");

    get_alias();

    wrefresh(loginWinBox);
}

std::string retrieve_alias()
{
    std::string temp(alias);

    return temp;
}