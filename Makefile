# Compiler selection - g++ (C++)
CC = g++

# Compiler flags
CXXFLAGS  = -Wall -Werror
CXXFLAGSDEBUG = -Wall -Werror -g -DDEBUG
CFLAGS = $(CXXFLAGS)

# Builds
DIRS = build build/bin
MKDIR_P = mkdir -p
RM_RF = rm -r
MOVE = mv

TARGET = popcl
SRC = pop3client/
OBJS_DIR = build/bin/
EXEC_DIR = build/
LIBS = -L/usr/lib/x86_64-linux-gnu/ -lcrypto -lssl

OBJECTS = main.o argumentParser.o pop3.o connection.o fileManipulator.o help.o

all: directory $(TARGET) moveToBuild

directory: $(DIRS)

moveToBuild: 
	mv *.o $(OBJS_DIR)
	cp $(TARGET) $(EXEC_DIR)

$(DIRS):
	$(MKDIR_P) $(DIRS)

$(TARGET): $(OBJECTS) $(SRC)returnCodes.hpp
	$(CC) $(CFLAGS) -o $(TARGET) main.o argumentParser.o pop3.o connection.o fileManipulator.o help.o $(LIBS)

main.o: main.cpp $(SRC)argumentParser.hpp $(SRC)pop3.hpp $(SRC)returnCodes.hpp
	$(CC) $(CFLAGS) -c main.cpp

argumentParser.o: $(SRC)argumentParser.cpp $(SRC)argumentParser.hpp $(SRC)help.hpp
	$(CC) $(CFLAGS) -c $(SRC)argumentParser.cpp

pop3.o: $(SRC)pop3.cpp $(SRC)pop3.hpp $(SRC)argumentParser.hpp $(SRC)connection.hpp $(SRC)fileManipulator.hpp $(SRC)returnCodes.hpp
	$(CC) $(CFLAGS) -c $(SRC)pop3.cpp

connection.o: $(SRC)connection.cpp $(SRC)connection.hpp $(SRC)help.hpp $(SRC)returnCodes.hpp
	$(CC) $(CFLAGS) -c $(SRC)connection.cpp

fileManipulator.o: $(SRC)fileManipulator.cpp $(SRC)fileManipulator.hpp $(SRC)help.hpp $(SRC)returnCodes.hpp
	$(CC) $(CFLAGS) -c $(SRC)fileManipulator.cpp

help.o: $(SRC)help.cpp $(SRC)help.hpp
	$(CC) $(CFLAGS) -c $(SRC)help.cpp

clean:
	$(RM_RF) $(EXEC_DIR)
	$(RM) $(TARGET)