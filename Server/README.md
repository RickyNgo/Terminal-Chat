## server.cpp && enki_daemon.cpp

### To Compile:
> make all</br>
> ./server

### Change Log:

#### 11.9
Processor class has replaced the handler and is designed to execute commands. It is working correctly and I think should make writing commands easier on the back end. I have 2 commands right now; stage and login. Stage is called as soon as the connection is made, and it stores the connection in a temporary buffer in the processor. Login is called when a message has the login command set. I do not have much of that command implemented, but the plan is to take the connection out of the temporary buffer and place it in a permanent data structure i.e. a set or map. In order to get this to work, I'm thinking that the stage command will send back a connection id to the client, in which, the client will send along with the alias they chose during login. However it happens though, the big thing is that these commands were pretty easy to write so far with the processor backbone in place.

There weren't too many major changes besides that. Take a look though and let me know if you guys have questions about any of it.

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
