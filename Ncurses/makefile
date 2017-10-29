CC=g++
CFLAGS=-std=c++11 
LDFLAGS=-lncurses

all: layout

layout: main.cpp layout.cpp layout.hpp
	$(CC) $(CFLAGS) main.cpp layout.cpp layout.hpp -o layout $(LDFLAGS)

