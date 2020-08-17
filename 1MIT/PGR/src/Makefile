##################################################
# nastaveni
CFLAGS_COMMON=-pipe -Wall -ggdb
CC=gcc
CXX=g++
NAME=main

##################################################
# nastaveni knihoven s OpenGL
ifeq ($(OS), Windows_NT)
  CFLAGS=$(CFLAGS_COMMON) -Wl,--enable-auto-import
  LIBS=-lopengl32 -lglew32 -lSDL
  RM=del
else
  USE_GLEE=1
  CFLAGS=$(CFLAGS_COMMON) `sdl-config --cflags` -I. -IGL -DUSE_GLEE
  LIBS=-L/usr/lib `sdl-config --libs` -lglut -lGL -lGLU
  RM=rm -f
endif

CXXFLAGS=$(CFLAGS) -std=c++11

##################################################
# obecne cile

DEPS=pgr.o Triangle.o Scene.o Hemicube.o Radiosity.o
ifdef USE_GLEE
  DEPS+=GLee.o
endif

.PHONY: all NAME clean

all: NAME

NAME: $(NAME)

run:
	./$(NAME) ../scene/scene_one_light.txt

clean:
	$(RM) *.o $(NAME)

##################################################
# pravidla

%: %.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

%: %.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

##################################################
# zavislosti

pgr.o: pgr.cpp pgr.h


main: main.cpp $(DEPS)