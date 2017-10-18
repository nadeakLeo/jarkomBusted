all :
		make src/sendfile src/recvfile

src/sendfile : src/sender.cpp src/bit.cpp src/bit.h src/dcomm.h
		g++ -pthread -o data/sendfile src/sender.cpp src/bit.cpp

src/recvfile : src/receiver.cpp src/bit.cpp src/bit.h src/dcomm.h
		g++ -pthread -o data/recvfile src/bit.cpp src/receiver.cpp
