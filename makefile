g++ -pthread -o receiver bit.cpp receiver.cpp
g++ -pthread -o transmitter bit.cpp sender.cpp
./receiver Message1.txt 4 10 8080
./transmitter Message.txt 4 10 1 8080
