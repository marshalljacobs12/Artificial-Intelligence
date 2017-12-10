CC = g++
CFLAGS = -O3
EXEC = PuzzleGenerator

all:
	rm -f $(EXEC)
	$(CC) $(CFLAGS) -o $(EXEC) main.cpp Puzzle.cpp PuzzleGenerator.cpp
	./$(EXEC) 10 10 1 9
