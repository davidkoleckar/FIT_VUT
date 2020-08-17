CC = g++
CFLAGS = -Wall -Wextra -pedantic
FILES = makefile webclient.cpp

all: webclient

webclient: webclient.cpp
	$(CC) $(CFLAGS) -o $@ webclient.cpp

pack:
	tar cvzf xkolec07.tar.gz webclient.cpp makefile