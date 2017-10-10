### To Compile:
> c++ -std=c++11 enki_daemon.cpp server.cpp -lboost_system -lboost_thread -lpthread </br>
> ./a.out

### Notes:
1. IOS is global now but should be moved to main. Need to find way to pass IOS to thread ios run function.
