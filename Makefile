CC=g++
CFLAGS=-std=c++11 -Wall

all: p1 p2

p1: p1.cpp
	@ mkdir -p out
	@ $(CC) $(CFLAGS) -o out/p1 p1.cpp

p2: p2.cpp
	@ mkdir -p out
	@ $(CC) $(CFLAGS) -o out/p2 p2.cpp

run-p1: p1
	./out/p1

run-p2: p2
	./out/p2

clean:
	rm -f out/p1 out/p2