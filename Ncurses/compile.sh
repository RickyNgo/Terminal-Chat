#!/bin/bash
killall -9 layout
killall -9 main
g++ -std=c++11 main.cpp layout.cpp layout.hpp -lncurses -lpanel -o main