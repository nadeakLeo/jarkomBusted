all :
		make sendfile recvfile

sendfile : src/sender.cpp src/bit.cpp src/bit.h src/dcomm.h
		g++ -pthread -o sendfile src/sender.cpp src/bit.cpp

recvfile : src/receiver.cpp src/bit.cpp src/bit.h src/dcomm.h
		g++ -pthread -o recvfile src/bit.cpp src/receiver.cpp
