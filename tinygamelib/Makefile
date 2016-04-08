CC=g++
CFLAGS=-c -Wall -std=c++11 $(shell sdl2-config --cflags) -I../..
LDFLAGS=$(shell sdl2-config --libs) -lSDL2_mixer

blagger: tinygamelib.o blagger.o
	$(CC) $(LDFLAGS) tinygamelib.o blagger.o -o blagger

tinygamelib.o: ../../tinygamelib.cpp
	$(CC) $(CFLAGS) ../../tinygamelib.cpp

blagger.o: blagger.cpp
	$(CC) $(CFLAGS) blagger.cpp

clean:
	rm -f blagger *.o
