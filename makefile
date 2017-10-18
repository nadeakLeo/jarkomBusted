g++ -pthread -o receiver bit.cpp receiver.cpp
g++ -pthread -o transmitter bit.cpp sender.cpp
./receiver Message1.txt 4 10 8080 & PIDIOS=$!
./transmitter Message.txt 4 10 127.0.0.1 8080 & PIDMIX=$!
wait $PIDIOS
wait $PIDMIX
