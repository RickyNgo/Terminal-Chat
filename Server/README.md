## server.cpp && enki_daemon.cpp

### To Compile:
> c++ -std=c++11 daemon.cpp server.cpp connection.cpp connection_handler.cpp messages.cpp channel.cpp -lboost_system -lpthread </br>
> ./a.out

### Change Log:

#### 10.26:
1. Reworked the server - contains the io_service now and created a stop asynch function to close connections when server shutsdown
2. Handler Class - this class contains channel and connection lists. User commands are sent to the handler. Handler processes the commands
here. The idea was to keep everything in one area.

User should be able to connect now and send a command. By this Saturday I'll finish the response member of the connection class so that the client receives a response about their request.


#### 10.10:
1. Fixed the Template Error
2. Ctrl-C now safely terminates program by calling io_service::stop
3. Renamed Backend enki main file to "enki_daemon.cpp"
4. Multiserver attempt on multserver branch - it compiles but the io_service is global. Need to find how to pass io_service object to thread function

#### 10.5:
1. Incomplete Accept Function -- Accepted Socket needs to be handled
2. Use Boost Exception? - http://www.boost.org/doc/libs/1_65_1/libs/exception/doc/boost-exception.html
3. Possible Template Endpoint for Channel and Server IO input
