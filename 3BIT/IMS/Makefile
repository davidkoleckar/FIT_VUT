CC=g++
CFLAGS=-Wall -Wextra -pedantic -std=c++11 -O2
LDFLAGS=-lsimlib -lm
SRC=model_restaurace.cc
PRJ=model

all:
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(PRJ)

run: 
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(PRJ)
	./$(PRJ)

pack:
	zip 03_xkolec07_xjanot02.zip model_restaurace.cc Makefile

unpack:
	unzip 03_xkolec07_xjanot02.zip

clean:
	rm -rf *.o

clean-all:
	rm -rf *.o $(PRJ)