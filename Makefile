all: server client validator 
client: client.cpp utils cryptogram
	g++ client.cpp utilities.o cryptogram.o -o client -lssl -lcrypto
server: server.cpp candidate utils cryptogram
	g++ server.cpp utilities.o candidate.o cryptogram.o -o server -lssl -lcrypto
validator: validator.cpp utils cryptogram
	g++ validator.cpp utilities.o cryptogram.o -o validator -lssl -lcrypto
candidate: candidate.cpp candidate.h utils
	g++ -c candidate.cpp -o candidate.o
utils: utilities.cpp utilities.h
	g++ -c utilities.cpp -o utilities.o
cryptogram: cryptogram.cpp cryptogram.h
	g++ -c cryptogram.cpp -o cryptogram.o  
.PHONY: clean
clean:
	rm *.o server validator client
