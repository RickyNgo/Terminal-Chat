#include "layout.hpp"
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

std::vector<std::string> time_buffer;
std::vector<std::string> alias_buffer;
std::vector<std::string> chat_buffer;
std::vector<std::string> contacts_buffer;

std::string current_channel = "None";


/* Implement in a later time. 
void draw_channels()
{
    channelWinBox = newwin(parent_y, channel_width, 0, 0);
    channelWin = newwin(parent_y - 2, channel_width - 2, 1, 1);

    box(channelWinBox, 124, 45);

    mvwprintw(channelWin, 0, 1, "Channels");

    scrollok(channelWinBox, TRUE);
    scrollok(channelWin, TRUE);

    wrefresh(channelWinBox);
    wrefresh(channelWin);
}
*/

void draw_chat()
{
    chatWinBox = newwin(parent_y-chat_box_height, parent_x - contact_width, 0, 0);
    chatWin = newwin(parent_y - chat_box_height - 2, parent_x - contact_width-2, 1, 0+1);

    /* Revert back to this whenever the Channel Window is added back in.
    chatWinBox = newwin(parent_y-chat_box_height, parent_x - channel_width - contact_width, 0, contact_width);
    chatWin = newwin(parent_y - chat_box_height - 2, parent_x - channel_width - contact_width-2, 1, contact_width+1);
    */
    box(chatWinBox, 124, 45);
    
    mvwprintw(chatWin, 0, 1, "CURRENT CHANNEL: %s", current_channel.c_str());
    
    scrollok(chatWinBox, TRUE);
    scrollok(chatWin, TRUE);
    
    wrefresh(chatWinBox);
    wrefresh(chatWin);
}

void draw_contacts()
{
    contactWinBox = newwin(parent_y, contact_width, 0, parent_x - contact_width);
    contactWin = newwin(parent_y -2, contact_width - 2, 1, parent_x - contact_width +1);

    box(contactWinBox, 124, 45);

    mvwprintw(contactWin, 0, 1, "Contacts");

    /*
    scrollok(contactWinBox, TRUE);
    scrollok(contactWin, TRUE);
    */

    wrefresh(contactWinBox);
    wrefresh(contactWin);
}

void draw_input()
{
    inputWinBox = newwin(chat_box_height, parent_x - contact_width, parent_y - chat_box_height, 0);
    inputWin = newwin(chat_box_height-2, parent_x - contact_width -2,  parent_y - chat_box_height + 1, 0 + 1);
    
    /* Shorten it when Channel Window is added back
    inputWinBox = newwin(chat_box_height, parent_x - channel_width - contact_width, parent_y - chat_box_height, channel_width);
    inputWin = newwin(chat_box_height-2, parent_x - channel_width - contact_width -2,  parent_y - chat_box_height + 1, channel_width + 1);
    */
    box(inputWinBox, 124, 45);
    
    mvwprintw(inputWin, 0, 1, ">:");

    /*
    scrollok(inputWinBox, TRUE);
    scrollok(inputWin, TRUE);
    */
    
    wrefresh(inputWinBox);
    wrefresh(inputWin);
}

int win_init()
{
    initscr();
    timeout(0);
    //halfdelay(10);
    start_color();
    srand(NULL);

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7, COLOR_CYAN, COLOR_BLACK);

    //keypad(stdscr, TRUE);
    getmaxyx(stdscr, parent_y, parent_x);

    draw_login();
    //draw_channels();
    draw_chat();
    draw_contacts();
    draw_input();


}

void del_wins()
{
    //delwin(channelWinBox);
    //delwin(channelWin);
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

    //draw_channels();
    draw_contacts();
    draw_chat();
    draw_input();

    display_chat();
    
    box(chatWinBox, 124, 45);
    wmove(inputWinBox, 1, 4);
    wclear(inputWinBox);
    box(inputWinBox, 124, 45);
    mvwprintw(inputWinBox, 1, 1, ">:");
    mvwprintw(chatWinBox, 1, 1, "CURRENT CHANNEL: ");

    wcursyncup(inputWinBox);
    //wrefresh(channelWinBox);
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

    wmove(inputWin, 0, 4);
    wrefresh(inputWin);
    
    int result = wgetnstr(inputWin, buffer, 280);

    if (result == ERR)
    {
        return "";
    }

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
        return "";
    }
    else
    {
        // The code below pretty much echos back what you type in
        /*
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
        */

        for_client.assign(buffer);
    }
    
    /////////////////////////////////////////////////////////
    // Keep this section as is to refresh the input window
    box(chatWinBox, 124, 45);
    //wmove(inputWinBox, 1, 4);
    wmove(inputWin, 0, 4);
    wclear(inputWin);
    //wclear(inputWinBox);
    //box(inputWinBox, 124, 45);
    mvwprintw(inputWin, 0, 1, ">:");
    wrefresh(inputWin);

    return for_client;
}

void display_chat()
{
    // I dont think this will need to be modified. The Client would put the contents of the Message received
    // from the server into the buffers.
    wclear(chatWin);
    int window_limit = parent_y - chat_box_height - 3;
    int chat_buffer_pos = chat_buffer.size()-1;

    int color;

    for (int i = chat_buffer.size(); i > 0; i--)
    {
        color = 7;

        mvwprintw(chatWin, window_limit, 1, time_buffer[chat_buffer_pos].c_str());

        wattron(chatWin, COLOR_PAIR(color));
        mvwprintw(chatWin, window_limit, 10, "<%s>", alias_buffer[chat_buffer_pos].c_str());
        wattroff(chatWin, COLOR_PAIR(color));
        
        mvwprintw(chatWin, window_limit, 26, "|%s", chat_buffer[chat_buffer_pos].c_str());

        window_limit--;
        chat_buffer_pos--;
        if (window_limit == 1)
        {
            break;
        }
    }

    mvwprintw(chatWin, 0, 1, "CURRENT CHANNEL: %s", current_channel.c_str());
    wrefresh(chatWin);
}


void splash_display()
{
    mvwprintw(chatWin, 10, parent_x/3.4+1, " _______ .__   __.  __  ___  __ ");
    mvwprintw(chatWin, 11, parent_x/3.4+1, "|   ____||  \\ |  | |  |/  / |  |");
    mvwprintw(chatWin, 12, parent_x/3.4+1, "|  |__   |   \\|  | |  '  /  |  |");
    mvwprintw(chatWin, 13, parent_x/3.4+1, "|   __|  |  . `  | |    <   |  |");
    mvwprintw(chatWin, 14, parent_x/3.4+1, "|  |____ |  |\\   | |  .  \\  |  |");
    mvwprintw(chatWin, 15, parent_x/3.4+1, "|_______||__| \\__| |__|\\__\\ |__|");
}

void show_help()
{
    int y;
    int x;

    getmaxyx(chatWinBox, y, x);
    WINDOW *helpWin = subwin(chatWinBox, 23, 90, y/3, x/3);

    keypad(helpWin, TRUE);
    
    std::string help = "\n"
                        " Commands are:\n"
                        " /help - Displays help commands and syntax.\n"
                        " /exit - Exits the Enki chat program.\n"
                        " /join [room] - Joins the chosen channel if available and creates it if it isn't there.\n"
                        " PRESS ANY KEY TO CLOSE THIS WINDOW";

    mvwprintw(helpWin, 0, 0, help.c_str());
    box(helpWin, 124, 45);

    wrefresh(helpWin);
    
    while(!wgetch(helpWin))
    {
        break;
    }

    delwin(helpWin);
    delwin(inputWin);
    delwin(inputWinBox);
    draw_input();
    draw_chat();
    display_chat();
}

void show_friends(std::vector<std::string> friends)
{
    for (int i = 0; i < friends.size(); i++)
    {
        mvwprintw(contactWinBox, 1, 2, friends[i].c_str());
    }
}

/*
void show_channels(std::vector<std::string> channels)
{
    for (int i = 0; i < channels.size(); i++)
    {
        mvwprintw(channelWinBox, 1, 2, channels[i].c_str());
    }
}
*/

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
        }       
        
        if (temp.length() > 4)
        {
            break;
        }

        wmove(loginWinBox, parent_y/2+3, parent_x/2-15);
        mvwprintw(loginWinBox, parent_y/2+3, parent_x/2-17, "    ");
        wrefresh(loginWinBox);
    

        wmove(loginWinBox, parent_y/2+3, parent_x/2-15);
        wrefresh(loginWinBox);

        int color = 2;

        wattron(loginWinBox, COLOR_PAIR(color));
        mvwprintw(loginWinBox, parent_y/2+2, parent_x/2-17, "INVALID LOGIN");
        wattroff(loginWinBox, COLOR_PAIR(color));
        
        wrefresh(loginWinBox);
    }
}

void draw_login()
{
    loginWinBox = newwin(0, 0, 0, 0);
    box(loginWinBox, 124, 45);

    mvwprintw(loginWinBox, parent_y/4, parent_x/2-18, " _______ .__   __.  __  ___  __ ");
    mvwprintw(loginWinBox, parent_y/4+1, parent_x/2-18, "|   ____||  \\ |  | |  |/  / |  |");
    mvwprintw(loginWinBox, parent_y/4+2, parent_x/2-18, "|  |__   |   \\|  | |  '  /  |  |");
    mvwprintw(loginWinBox, parent_y/4+3, parent_x/2-18, "|   __|  |  . `  | |    <   |  |");
    mvwprintw(loginWinBox, parent_y/4+4, parent_x/2-18, "|  |____ |  |\\   | |  .  \\  |  |");
    mvwprintw(loginWinBox, parent_y/4+5, parent_x/2-18, "|_______||__| \\__| |__|\\__\\ |__|");



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

void update_buffers(std::string time, std::string alias, std::string chat)
{
    time_buffer.push_back(time);
    alias_buffer.push_back(alias);
    chat_buffer.push_back(chat);
}

void update_contacts(std::string list)
{
    std::string delimiter = "|";
    std::string token;

    int pos = 0;

    while((pos = list.find(delimiter)) != std::string::npos)
    {
        token = list.substr(0, pos);
        contacts_buffer.push_back(token);
        list.erase(0, pos + delimiter.length());
    }

    contacts_buffer.push_back(token);
}

void update_current_channel(std::string channel)
{   
    std::string delimiter = " ";
    std::string token;

    token = channel.substr(0, channel.find(delimiter));

    if (token == " ")
    {
        token = channel.substr(0, channel.find(delimiter));
    }

    current_channel.assign(token);
}