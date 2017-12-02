#ifndef layout
#define layout

#include <ncurses.h>
#include <vector>
#include <string>

int win_init();
void del_wins();

void resize_handler(int signum);
void resize();
void reset_resize_flag();
int get_resize_flag();

void draw_channels();
void draw_chat();
void draw_contacts();
void draw_input();
void draw_login();
void display_chat();

void get_alias();
std::string retrieve_alias();
std::string get_input();

void show_help();
void show_friends();
void show_channels(std::vector<std::string> channels);

int invite_notification(std::vector<std::string> inviter);

void update_buffers(std::string time, std::string alias, std::string chat);
void update_contacts(std::string list);
void update_current_channel(std::string channel);
void clear_buffers();

#endif