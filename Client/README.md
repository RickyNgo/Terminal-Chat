c++ -std=c++11 channel.cpp client.cpp db_helper.cpp main.cpp -o chat_client -lboost_system -lpthread
./chat_client <port>



Connects by default to localhost.

db_helper will hold database interactions once set up.
