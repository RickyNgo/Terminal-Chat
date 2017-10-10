## server.cpp & enki_backend.cpp - Written by Brian


### Change Log:

#### 10.10:
Compiles and Runs. Use:
> c++ -std=c++11 enki_backend.cpp server.cpp -lboost_system -lpthread
executable is a.out.

#### 10.5:
1. Incomplete Accept Function -- Accepted Socket needs to be handled
2. Use Boost Exception? - http://www.boost.org/doc/libs/1_65_1/libs/exception/doc/boost-exception.html
3. Possible Template Endpoint for Channel and Server IO input
