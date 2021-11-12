# Compiler selection - g++ (C++)
CC = g++

# Compiler flags
CXXFLAGS  = -Wall -Werror
CXXFLAGSDEBUG = -Wall -Werror -g -DDEBUG
CFLAGS = $(CXXFLAGSDEBUG)

# Builds
TARGET = popcl

all: $(TARGET)

$(TARGET): main.o argumentsParser.o help.o returnCodes.hpp
	$(CC) $(CFLAGS) -o $(TARGET) main.o argumentsParser.o help.o

main.o: main.cpp argumentsParser.hpp
	$(CC) $(CFLAGS) -c main.cpp

argumentsParser.o: argumentsParser.cpp argumentsParser.hpp
	$(CC) $(CFLAGS) -c argumentsParser.cpp

help.o: help.cpp help.hpp
	$(CC) $(CFLAGS) -c help.cpp

clean:
	$(RM) $(TARGET) *.o