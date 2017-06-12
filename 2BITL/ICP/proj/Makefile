export PATH := /usr/local/share/Qt-5.5.1/5.5/gcc_64/bin:$(PATH)
export LD_LIBRARY_PATH := /usr/local/share/Qt-5.5.1/5.5/gcc_64/lib:$(LD_LIBRARY_PATH)

CC=g++ -std=c++11
FLAGS=-Wextra -pedantic -Wall -g -lboost_serialization -fPIC

all: hra2016-cli hra2016

artificial_intelligence.o:
	cd src;$(CC) artificial_intelligence.cpp -c $(FLAGS)
board_history.o:
	cd src;$(CC) board_history.cpp -c $(FLAGS)
io.o:
	cd src;$(CC) io.cpp -c $(FLAGS)
builder.o:
	cd src;$(CC) builder.cpp -c $(FLAGS)
cli.o:
	cd src;$(CC) cli.cpp -c $(FLAGS)
core.o:
	cd src;$(CC) core.cpp -c $(FLAGS)
grey_manager.o:
	cd src;$(CC) grey_manager.cpp -c $(FLAGS)
messenger.o:
	cd src;$(CC) messenger.cpp -c $(FLAGS)
othello.o:
	cd src;$(CC) othello.cpp -c $(FLAGS)
scout.o:
	cd src;$(CC) scout.cpp -c $(FLAGS)
	
hra2016-cli:artificial_intelligence.o board_history.o core.o grey_manager.o messenger.o othello.o scout.o builder.o io.o
	$(CC) src/scout.o src/othello.o src/messenger.o src/io.o src/grey_manager.o src/core.o src/builder.o src/artificial_intelligence.o src/board_history.o src/cli_main.cpp src/cli.cpp -o src/hra2016-cli $(FLAGS)
	
hra2016:
	cd src/ ; \
	qmake hra2016.pro ;\
	make ;
	cd ..

clean:
	cd src; make clean; cd .. ; \
	rm  src/Makefile src/hra2016-cli src/hra2016

gui:
	cd src; make

run:
	src/hra2016-cli && src/hra2016;

doxygen:
	doxygen ./src/othello.cfg
