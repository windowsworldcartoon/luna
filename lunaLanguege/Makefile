CC=g++
CFLAGS=-std=c++11 -Wall

all: luna

luna lexer.o parser.o runtime.o
    $(CC) $(CFLAGS) -o mylanguage lexer.o parser.o runtime.o

lexer.o: lexer.cpp
    $(CC) $(CFLAGS) -c lexer.cpp

parser.o: parser.cpp
    $(CC) $(CFLAGS) -c parser.cpp

runtime.o: runtime.cpp
    $(CC) $(CFLAGS) -c runtime.cpp

clean:
    rm -f *.o mylanguage