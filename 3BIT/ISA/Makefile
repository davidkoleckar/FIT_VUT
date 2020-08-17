CC = g++
CFLAGS = -Wall -Wextra -pedantic
FILES = Makefile fclient.cpp

all: fclient

fclient: fclient.cpp
	$(CC) $(CFLAGS) -o $@ fclient.cpp

pack:
	tar cfv xkolec07.tar fclient.cpp Makefile README manual.pdf