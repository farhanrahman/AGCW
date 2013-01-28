programName := CW1
programSRC := $(wildcard *.cpp)
programOBJS := main.o loadPNM.o

CC := g++

.PHONY: all clean

all: $(programName)

$(programName): $(programOBJS)
	 $(LINK.cc) $(programOBJS) -o $(programName)

main.o: main.cpp
	$(LINK.cc) -c -o main.o main.cpp

loadPNM.o: loadPNM.cpp
	$(LINK.cc) -c -o loadPNM.o loadPNM.cpp

clean:
	@- rm $(programOBJS)
	@- rm $(programName)
