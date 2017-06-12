OBJC= server.o

CC=g++
CFLAGS=-Wall -Wextra -pedantic -lstdc++

.PHONY: build
.PHONY: clean


build: server

server: server.cpp
	$(CC) $(CFLAGS) -o $@ server.cpp
	
clean:
	rm -f *.o server
