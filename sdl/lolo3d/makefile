CPP=g++
CPPFLAGS=$(shell sdl2-config --cflags) -c -Wall -std=c++11
LDLIBS=$(shell sdl2-config --libs) -lSDL2_image -framework OpenGL -lglew -ltinyxml2

PROGRAM=lolo3d
SOURCES=main.cpp glsl_utils.cpp game.cpp map.cpp object3d.cpp wavefront_loader.cpp map_loader.cpp level.cpp object.cpp
OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CPP) $(LDLIBS) $(OBJECTS) -o $@

.cpp.o:
	$(CPP) $(CPPFLAGS) $< -o $@

clean:
	rm -rf *.o $(PROGRAM)
