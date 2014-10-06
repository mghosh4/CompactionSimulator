CC=g++
CFLAGS=-g -Wno-deprecated -DDEBUG
LDFLAGS= -lm -lboost_serialization
SRC=\
	src/timer.cpp \
	src/fileparser.cpp \
	src/kwaymerge.cpp \
	src/sizetierstrategy.cpp \
	src/greedystrategy.cpp
OBJ=$(SRC:.cpp=.o)

all: main

main: $(OBJ)
	$(CC) $(CFLAGS) src/main.cpp $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -f src/*.o main
