#!/bin/bash

killall -9 layout

g++ -std=c++11 layout.cpp layout.hpp -lncurses -lpanel -o layout