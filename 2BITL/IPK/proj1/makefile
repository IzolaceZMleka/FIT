NAME=webclient
 
CC=g++
CFLAGS=-Wall -pedantic -g -std=c++11 -Wl,-rpath=/usr/local/lib/gcc49/


$(NAME): webclient.cpp
	$(CC) $(CFLAGS) webclient.cpp -o $(NAME)
	
clean:
	rm -f *~ *.bak
	rm -f $(NAME)
