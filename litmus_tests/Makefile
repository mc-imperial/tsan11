CC=clang++
FLAGS=-std=c++0x -pthread -Wall $(SANITIZE) -g
SOURCES=$(wildcard *.cpp)
PROGRAMS=$(SOURCES:.cpp=)

all: $(PROGRAMS)

.cpp:
	$(CC) $(FLAGS) $< -o $@

clean:
	rm -f $(PROGRAMS)
