all: server client validator
client: client.cpp utils
	g++ client.cpp utilities.o -o client
server: server.cpp utils
	g++ server.cpp utilities.o -o server
validator: server.cpp utils
	g++ validator.cpp utilities.o -o validator
utils: utilities.cpp utilities.h
	g++ -c  utilities.cpp -o utilities.o
.PHONY: clean
clean:
	rm *.o server validator client
