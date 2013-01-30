programName := CW1
programSRC := $(wildcard *.cpp)
programOBJS := main.o loadPNM.o util.o image.o HDRCompiler.o

CXX := g++
CC := gcc

.PHONY: all clean

all: $(programName)

$(programName): $(programOBJS)
	 $(LINK.cc) $(programOBJS) -o $(programName)

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

clean:
	@- rm $(programOBJS)
	@- rm $(programName)
