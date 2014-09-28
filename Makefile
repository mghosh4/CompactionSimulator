CC=g++
CFLAGS=-g -Wno-deprecated -DDEBUG
LDFLAGS= -lm #-lboost_serialization
SRC=\
	fileparser.cpp \
	kwaymerge.cpp \
	strategy.cpp
OBJ=$(SRC:.cpp=.o)

all: main

main: $(OBJ)
	$(CC) $(CFLAGS) main.cpp $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -f *.o main
