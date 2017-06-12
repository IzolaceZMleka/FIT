CC=g++
CFLAGS=-Wall -Wextra -pedantic -lstdc++

.PHONY: build
.PHONY: clean


build: client

client: client.cpp
	$(CC) $(CFLAGS) -o $@ client.cpp
	
clean:
	rm -f *.o client
