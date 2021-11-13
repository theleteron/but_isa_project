# Compiler selection - g++ (C++)
CC = g++

# Compiler flags
CXXFLAGS  = -Wall -Werror
CXXFLAGSDEBUG = -Wall -Werror -g -DDEBUG
CFLAGS = $(CXXFLAGSDEBUG)

# Builds
TARGET = popcl

all: $(TARGET)

$(TARGET): main.o argumentParser.o help.o returnCodes.hpp
	$(CC) $(CFLAGS) -o $(TARGET) main.o argumentParser.o help.o

main.o: main.cpp argumentParser.hpp
	$(CC) $(CFLAGS) -c main.cpp

argumentParser.o: argumentParser.cpp argumentParser.hpp help.hpp
	$(CC) $(CFLAGS) -c argumentParser.cpp

help.o: help.cpp help.hpp
	$(CC) $(CFLAGS) -c help.cpp

clean:
	$(RM) $(TARGET) *.o