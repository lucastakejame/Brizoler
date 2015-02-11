CC = g++
LD = g++

CFLAGS = -Wall -Wextra -Wpointer-arith -Wredundant-decls
CFLAGS += -Wcast-align -Wcast-qual

LDFLAGS = -lSDLmain -lSDL

EXEC = brizoler

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

all: $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $(EXEC)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm *.o
	rm $(EXEC)