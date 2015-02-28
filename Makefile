all: server client validator
client: client.cpp utils
	g++ client.cpp utilities.o -o client
server: server.cpp candidate utils
	g++ server.cpp utilities.o -o server
validator: validator.cpp utils
	g++ validator.cpp utilities.o -o validator
candidate: candidate.cpp candidate.h utils
	g++ -c candidate.cpp -o candidate.o
utils: utilities.cpp utilities.h
	g++ -c  utilities.cpp -o utilities.o 
.PHONY: clean
clean:
	rm *.o server validator client
