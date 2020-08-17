CC=g++
CFLAGS = -std=c++11 -Wall -Wextra
RM = rm -rf

SOURCES_CODEC = huff_codec.cpp
HEADER_CODEC = huff_codec.hpp

all: huff_codec

huff_codec: 
	$(CC) $(CFLAGS) $(HEADER_CODEC) $(SOURCES_CODEC) -o $@

clean:
	$(RM) huff_codec