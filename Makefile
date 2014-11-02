CC=g++
CFLAGS=-g -Wno-deprecated -DDEBUG
LDFLAGS= -lm -lboost_serialization
SRC=\
	src/external/murmur3.cpp \
	src/utilities/timer.cpp \
	src/utilities/utilities.cpp \
	src/utilities/kwaymerge.cpp \
	src/parser/ycsbparser.cpp \
	src/strategy/sizetierednumberstrategy.cpp \
	src/strategy/greedynumberstrategy.cpp \
	src/strategy/sizetieredfilestrategy.cpp \
	src/strategy/greedyfilestrategy.cpp \
	src/compaction/compaction.cpp 

OBJ=$(SRC:.cpp=.o)

all: main

main: $(OBJ)
	$(CC) $(CFLAGS) src/main.cpp $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -f $(OBJ) main
