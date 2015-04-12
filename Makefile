CC = g++
LD = g++

DEGUB = -g

CFLAGS = -Wall -Wextra -Wpointer-arith -Wredundant-decls
CFLAGS += -Wcast-align -Wcast-qual
CFLAGS += $(DEBUG)

LDFLAGS = -lSDLmain -lSDL

EXEC = brizoler.bin

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

all: $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $(EXEC)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm *.o
	rm $(EXEC)
