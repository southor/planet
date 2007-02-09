.PHONY: all

CCC = g++

CCFLAGS = -I/student/include -I../lib/include -Wall `sdl-config --cflags`
LDFLAGS = -L/student/lib -L../lib/lib -lGLU -lGL -lrt -lSDL_image -lSDL_mixer `sdl-config --libs`

all: main

clean:
	rm -f *.o

%.o: %.cpp
	$(CCC) -c -o $@ $< $(CCFLAGS)

main: main.o djungelfeber.o gameobject.o heightmap.o splinepoint.o camera.o light.o particleeffect.o particle.o particlecloud.o blob.o lighthandler.o
	$(CCC) -o $@ $^ $(LDFLAGS)

