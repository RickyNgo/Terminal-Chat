#ifndef layout_hpp
#define layout_hpp

#include <ncurses.h>
#include <vector>
#include <string>

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
void show_help();
void show_friends(std::vector<std::string> friends);
void show_channels(std::vector<std::string> channels);
int invite_notification();


#endif