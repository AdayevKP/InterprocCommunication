CC=g++ 
CFLAGS=-Wall -O3
LIBS=-lpthread -lrt 
GAME_FILE=main.cpp 

CONN_FILES=$(shell find . -name "conn_*.cpp")

EXECUTABLES=$(CONN_FILES:./conn_%.cpp=game_%)

all: $(EXECUTABLES)

game_%: conn_%.o $(GAME_FILE:.cpp=.o)
	$(CC) -o $@ $^ $(LIBS) 

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ 

.PHONY: clean

clean:
	rm -f `find . -maxdepth 1 -executable -type f`
