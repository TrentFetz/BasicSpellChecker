CC = g++
CFLAGS = -std=c++11 -Wall

all: output.x 

output.x: myspell.cpp HashTable.h
	$(CC) $(CFLAGS) -o output.x myspell.cpp

clean:
	rm -f output.x *.x


