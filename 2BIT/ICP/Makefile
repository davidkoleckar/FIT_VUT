# ICP Reversi

QMAKE = "/usr/local/share/Qt-5.2.1/5.2.1/gcc_64/bin/qmake"

all: gui cli

#build gui
gui:
	@cd src && qmake; make
	mv src/reversi ./hra2016


#cli
cli:
	g++ -std=c++11 -Wall -Wextra -pedantic -lboost_filesystem -lboost_thread-mt src/include/Rules.h src/include/Field.h src/include/BorderField.h src/src/Board.cpp src/src/BoardField.cpp src/src/Disk.cpp src/src/Game.cpp src/src/newGame.cpp src/src/Player.cpp src/src/ReversiRules.cpp src/cliGame.cpp  -o hra2016-cli


#run 
run: all
	./hra2016
	./hra2016-cli

doxygen:
	-mkdir doc
	doxygen src/doxyfile

clean:
	rm src/*.o -rf
	rm hra2016 -rf
	rm hra2016-cli -rf
	rm doc/* -rf
	@cd src && make clean && make mocclean && make distclean
	-rmdir doc

pack:
	zip xkolec07-xjanot02.zip Makefile src