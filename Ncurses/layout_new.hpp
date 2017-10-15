#ifndef layout_hpp
#define layout_hpp

#include <ncurses.h>
#include <vector>
#include <string>

// Windows to be used
//WINDOW *chatWin, *chatWinBox, *channelWin, *channelWinBox, *inputWin, *inputWinBox, *contactWin, *contactWinBox;

// Global window parameters
/*
int parent_x;
int parent_y;
int new_x;
int new_y;

int channel_width = 30;
int contact_width = 30;
int chat_box_height = 5;

int channel_list_yindex = 2;
int dialog_list_yindex = 2;
int contact_list_yindex = 2;*/
//WINDOW *chatWin, *chatWinBox, *channelWin, *channelWinBox, *inputWin, *inputWinBox, *contactWin, *contactWinBox;

int win_init();
void del_wins();
void resize_handler(int signum);
void resize();
void draw_channels();
void draw_chat();
void draw_contacts();
void draw_input();
void get_input();
void display_chat();
void splash_display();


#endif