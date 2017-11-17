all: main.cpp client.cpp channel.cpp messages.cpp
	c++ -std=c++11 main.cpp client.cpp channel.cpp messages.cpp -o client -lboost_system -lpthread

ncurses: unified_main.cpp client.cpp aux_functions.cpp channel.cpp messages.cpp layout.cpp
	c++ -std=c++11 unified_main.cpp client.cpp aux_functions.cpp channel.cpp messages.cpp layout.cpp -o main -lboost_system -lpthread -lncurses
