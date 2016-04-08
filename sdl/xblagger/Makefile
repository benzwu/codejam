CC=g++
CFLAGS=-c -Wall -std=c++11 $(shell sdl2-config --cflags) -I/usr/local/include
LDFLAGS=$(shell sdl2-config --libs) -lSDL2_mixer -ltinyxml2

xblagger: xblagger.o datamanager.o level.o object.o
	$(CC) $(LDFLAGS) xblagger.o datamanager.o level.o object.o -o xblagger

xblagger.o: xblagger.cpp
	$(CC) $(CFLAGS) xblagger.cpp

datamanager.o: datamanager.cpp
	$(CC) $(CFLAGS) datamanager.cpp

level.o: level.cpp
	$(CC) $(CFLAGS) level.cpp

object.o: object.cpp
	$(CC) $(CFLAGS) object.cpp

clean:
	rm -f xblagger xblagger.o datamanager.o level.o object.o
