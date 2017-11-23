CXX=g++
CFLAGS=-std=c++14 
LDFLAGS=-lncurses
LDFLAGS+=-lboost_system
LDFLAGS+=-lpthread
LDFLAGS+=-lboost_thread

LayoutSrc=layout.cpp
ClientSrc=aux_functions.cpp client.cpp db_helper.cpp unified_main.cpp channel.cpp
MessageSrc=messages.cpp
ServerSrc=channel_utility.cpp channel.cpp connection.cpp daemon.cpp guest.cpp processor.cpp server.cpp session.cpp

LayoutDir=Ncurses/
ClientDir=Client/
MessageDir=Messages/
ServerDir=Server/

LayoutPath=$(addprefix $(LayoutDir),$(LayoutSrc))
ClientPath=$(addprefix $(ClientDir),$(ClientSrc))
MessagePath=$(addprefix $(MessageDir),$(MessageSrc))
ServerPath=$(addprefix $(ServerDir),$(ServerSrc))

enki_client: $(ClientPath) $(MessagePath) $(LayoutPath)
	$(CXX) $(CFLAGS) $(ClientPath) $(MessagePath) $(LayoutPath) -o enki_client $(LDFLAGS) 


enki_server: $(ServerPath) $(MessagePath)
	$(CXX) $(CFLAGS) $(ServerPath) $(MessagePath) -o enki_server $(LDFLAGS) 
