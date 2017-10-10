#!/bin/bash

killall -9 ntest
killall -9 vtest
g++ -std=c++11 ncurses_test.cpp ncurses_test.hpp -lncurses -lpanel -o ntest
g++ -std=c++11 viget_sample.cpp -lncurses -lpanel -o vtest
