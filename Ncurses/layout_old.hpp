#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include <ncurses.h>
#include <vector>
#include <string>
//#include "messages.hpp"
class UI
{
    public:
		void draw_borders(WINDOW *screen);
		int windows_init();
		void resize();
		void delete_windows();
		void side_display(WINDOW *screen, std::vector<std::string> list);
        void chat_display(std::vector<std::string> list);
        void splash_display();
		void content_update();
		void get_input();

    private:
		int parent_x;
		int parent_y;
		int new_x;
		int new_y;

		int channel_width = 30;
		int contact_width = 30;
		int chat_box_height = 5;

		int channel_list_yindex = 2;
		int dialog_list_yindex = 2;
		int contact_list_yindex = 2;

		WINDOW *channels;
		WINDOW *dialog;
		WINDOW *contacts;
		WINDOW *chat_box;
};

#endif