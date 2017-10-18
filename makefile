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
