all: main.cpp client.cpp channel.cpp messages.cpp
	c++ -std=c++11 main.cpp client.cpp channel.cpp messages.cpp -o client -lboost_system -lpthread

ncurses: main_ncurses.cpp client.cpp aux_functions.cpp channel.cpp messages.cpp layout.cpp
	c++ -std=c++11 main_ncurses.cpp ncurses_client.cpp aux_functions.cpp channel.cpp messages.cpp layout.cpp -o ncurses_client -lboost_system -lpthread -lncurses
