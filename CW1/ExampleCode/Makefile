partOne := partOne
partTwo := partTwo

programCPPSRC := $(wildcard *.cpp)

programCCSRC := $(wildcard *.cc)

programOBJS := ${programCPPSRC:.cpp=.o} ${programCCSRC:.cc=.o}

partOneOBJS := main.o loadPNM.o util.o image.o HDRCompiler.o

partTwoOBJS := image.o scene_parser.o sphere.o group.o orthographic_camera.o part2.o loadPNM.o util.o

CC      = g++
LD      = g++

CFLAGS  = -I. -c -g

LDFLAGS = -lm

INCS = EnvironmentMap.h hit.h image.h ray.h scene_parser.h vectors.h \
	object3d.h group.h camera.h orthographic_camera.h

partTwoOBJS =	image.o scene_parser.o sphere.o group.o orthographic_camera.o part2.o loadPNM.o util.o



CXX := g++
CC := g++

.PHONY: all clean

all: $(partOne) $(partTwo)

$(partOne): $(partOneOBJS)
	 $(LINK.cc) $(partOneOBJS) -o $(partOne)

main.o: main.cpp
	$(LINK.cc) -c -o main.o main.cpp

loadPNM.o: loadPNM.cpp
	$(LINK.cc) -c -o loadPNM.o loadPNM.cpp

util.o: util.cpp
	$(LINK.cc) -c -o util.o util.cpp

image.o: image.cpp
	$(LINK.cc) -c -o image.o image.cpp

HDRCompiler.o: HDRCompiler.cpp
	$(LINK.cc) -c -o HDRCompiler.o HDRCompiler.cpp

orthographic_camera.o: $(INCS) orthographic_camera.cc
	$(CC) $(CFLAGS) orthographic_camera.cc -o orthographic_camera.o

scene_parser.o: $(INCS) scene_parser.cc
	$(CC) $(CFLAGS) scene_parser.cc -o scene_parser.o

sphere.o: $(INCS) sphere.cc
	$(CC) $(CFLAGS) sphere.cc -o sphere.o

group.o: $(INCS) group.cc
	$(CC) $(CFLAGS) group.cc -o group.o

part2.o: $(INCS) part2.cpp
	$(CC) $(CFLAGS) part2.cpp -o part2.o

$(partTwo): $(partTwoOBJS)
	$(LD) $(partTwoOBJS) -o $(partTwo) $(LDFLAGS)


clean:
	@- rm $(programOBJS)
	@- rm $(partOne)
	@- rm $(partTwo)

