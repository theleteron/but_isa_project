# Compiler selection - g++ (C++)
CC = g++

# Compiler flags
CXXFLAGS  = -Wall -Werror
CXXFLAGSDEBUG = -Wall -Werror -g -DDEBUG
CFLAGS = $(CXXFLAGSDEBUG)

# Builds
TARGET = popcl

all: $(TARGET)

$(TARGET): main.o argumentParser.o pop3.o connection.o fileManipulator.o help.o pop3client/returnCodes.hpp
	$(CC) $(CFLAGS) -o $(TARGET) main.o argumentParser.o pop3.o connection.o fileManipulator.o help.o

main.o: main.cpp pop3client/argumentParser.hpp pop3client/pop3.hpp pop3client/returnCodes.hpp
	$(CC) $(CFLAGS) -c main.cpp

argumentParser.o: pop3client/argumentParser.cpp pop3client/argumentParser.hpp pop3client/help.hpp
	$(CC) $(CFLAGS) -c pop3client/argumentParser.cpp

pop3.o: pop3client/pop3.cpp pop3client/argumentParser.hpp pop3client/connection.hpp pop3client/fileManipulator.hpp
	$(CC) $(CFLAGS) -c pop3client/pop3.cpp

connection.o: pop3client/connection.cpp pop3client/help.hpp pop3client/returnCodes.hpp
	$(CC) $(CFLAGS) -c pop3client/connection.cpp

fileManipulator.o: pop3client/fileManipulator.cpp pop3client/help.hpp pop3client/returnCodes.hpp
	$(CC) $(CFLAGS) -c pop3client/fileManipulator.cpp

help.o: pop3client/help.cpp pop3client/help.hpp
	$(CC) $(CFLAGS) -c pop3client/help.cpp

clean:
	$(RM) $(TARGET) *.o