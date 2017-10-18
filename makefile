# g++ -pthread -o receiver bit.cpp receiver.cpp
# g++ -pthread -o transmitter bit.cpp sender.cpp
# ./receiver Message1.txt 4 10 8080 & PIDIOS=$!
# ./transmitter Message.txt 4 10 127.0.0.1 8080 & PIDMIX=$!
# wait $PIDIOS
# wait $PIDMIX


output: receiver transmitter
	./receiver Message1.txt 4 10 8080 & PIDIOS=$!
	./transmitter Message.txt 4 10 127.0.0.1 8080 & PIDMIX=$!
	wait $PIDIOS
	wait $PIDMIX
receiver:
	g++ -pthread -o receiver bit.cpp receiver.cpp
transmitter: 
	g++ -pthread -o transmitter bit.cpp sender.cpp
clean:
	rm receiver transmitter
all :
		make src/sendfile src/recvfile

src/sendfile : src/sender.cpp src/bit.cpp src/bit.h src/dcomm.h
		g++ -pthread -o data/sendfile src/sender.cpp src/bit.cpp

src/recvfile : src/receiver.cpp src/bit.cpp src/bit.h src/dcomm.h
		g++ -pthread -o data/recvfile src/bit.cpp src/receiver.cpp

		./receiver Message1.txt 4 10 8080 & PIDIOS=$!
		./transmitter Message.txt 4 10 127.0.0.1 8080 & PIDMIX=$!
		wait $PIDIOS
		wait $PIDMIX
