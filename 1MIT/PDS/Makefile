CC=g++
CFLAGS = -std=c++11 -Wall -Wextra -pedantic -pthread
RM = rm -rf

SOURCES_STARVE = dhcpstarve.cpp
SOURCES_ROGUE = dhcprogue.cpp
SOURCES_COMMON = error.cpp network.cpp

HEADER_STARVE = dhcpstarve.h
HEADER_ROGUE = dhcprogue.h
HEADER_COMMON = error.h network.h

all: pds-dhcpstarve pds-dhcprogue

pds-dhcpstarve: 
	$(CC) $(CFLAGS) $(HEADER_STARVE) $(HEADER_COMMON) $(SOURCES_STARVE) $(SOURCES_COMMON) -o $@

pds-dhcprogue: 
	$(CC) $(CFLAGS) $(HEADER_ROGUE) $(HEADER_COMMON) $(SOURCES_ROGUE) $(SOURCES_COMMON) -o $@ 

clean:
	$(RM) pds-dhcpstarve pds-dhcprogue

pack:
	zip xkolec07.zip $(SOURCES_STARVE) $(SOURCES_ROGUE) $(SOURCES_COMMON) $(HEADER_ROGUE) $(HEADER_STARVE) $(HEADER_COMMON) readme.txt dokumentace.pdf

