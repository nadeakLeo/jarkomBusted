all :
		g++ -pthread -o sendfile src/sender.cpp src/bit.cpp
		g++ -pthread -o recvfile src/bit.cpp src/receiver.cpp
