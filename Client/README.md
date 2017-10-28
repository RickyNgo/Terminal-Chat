c++ -std=c++11 channel.cpp client.cpp aux_functions.cpp messages.cpp main.cpp -o chat_client -lboost_system -lpthread

./chat_client <port>

testing with testmain.cpp

c++ -std=c++11 channel.cpp client.cpp aux_functions.cpp messages.cpp testmain.cpp -o chat_test -lboost_system -lpthread

./chat_test


Connects by default to localhost.


OLD:
db_helper will hold database interactions once set up.
