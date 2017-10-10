//#include <ncurses.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <panel.h>
#include <ctime>
#include <cstring>
#include "ncurses_test.hpp"


int main()
{
	int parent_x;
	int parent_y;
	int new_x;
	int new_y;
	int score_size = 10;

	int dialog_x = 1;
	int dialog_y = 2;

	initscr();
	//noecho();
	//curs_set(FALSE);
	cbreak();
	getmaxyx(stdscr, parent_y, parent_x);

	WINDOW *dialog = newwin(parent_y-score_size, parent_x, 0, 0);
	WINDOW *chatbox = newwin(score_size, parent_x, parent_y-score_size, 0);
	
	draw_borders(dialog);
	draw_borders(chatbox);

	wmove(chatbox, 1, 7);

	int count = 0;
	while(1)
	{
		getmaxyx(stdscr, new_y, new_x);
		
		if (new_y != parent_y || new_x != parent_x)
		{
			parent_x = new_x;
			parent_y = new_y;


			wresize(dialog, new_y, new_x);
			wresize(chatbox, score_size, new_x);
			mvwin(chatbox, new_y-score_size, 0);

			wclear(stdscr);
			wclear(dialog);
			wclear(chatbox);

			draw_borders(dialog);
			draw_borders(chatbox);
		}
		

		mvwprintw(dialog, 1, 1, "Dialog");
		mvwprintw(chatbox, 1, 1, "Chat");
		mvwprintw(chatbox, 1, 5, ": ");
		char chat_input[256];

		wmove(chatbox, 1, 7);
	
		wrefresh(stdscr);
		wrefresh(dialog);
		wrefresh(chatbox);
		
		
		wgetnstr(chatbox, chat_input, 255);
		wclear(chatbox);
		draw_borders(chatbox);

		const time_t ctt = time(0);
		char* local_time = asctime(localtime(&ctt));
		//strcat(local_time, ": ");
		int chat_spacer = strlen(local_time) + 4;
		

		mvwprintw(dialog, dialog_y, dialog_x, local_time);
		
		mvwprintw(dialog, dialog_y, dialog_x+ chat_spacer, chat_input);
		
		dialog_y++;
		sleep(1);
	}

	delwin(dialog);
	delwin(chatbox);
	endwin();

	return 0;
}
