CC=g++
CFLAGS=-g -Wno-deprecated -DDEBUG
LDFLAGS= -lm -lboost_thread-mt
SRC=\
	src/external/murmur3.cpp \
	src/utilities/timer.cpp \
	src/utilities/utilities.cpp \
	src/utilities/kwaymerge.cpp \
	src/parser/ycsbparser.cpp \
	src/strategy/numberstrategy/sizetierednumberstrategy.cpp \
	src/strategy/numberstrategy/greedynumberstrategy.cpp \
	src/strategy/numberstrategy/btsizetierednumberstrategy.cpp \
	src/strategy/numberstrategy/btgreedynumberstrategy.cpp \
	src/strategy/numberstrategy/randomnumberstrategy.cpp \
	src/strategy/filestrategy/sizetieredfilestrategy.cpp \
	src/strategy/filestrategy/greedyfilestrategy.cpp \
	src/strategy/filestrategy/btsizetieredfilestrategy.cpp \
	src/strategy/filestrategy/btgreedyfilestrategy.cpp \
	src/strategy/filestrategy/randomfilestrategy.cpp \
	src/compaction/compaction.cpp 

OBJ=$(SRC:.cpp=.o)

all: main

main: $(OBJ)
	$(CC) $(CFLAGS) src/main.cpp $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -f $(OBJ) main
