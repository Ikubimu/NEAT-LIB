CC = g++

SOURCES = main.cpp src/NEAT-LIB.cpp src/common.cpp src/GENOME/genome.cpp src/GENOME/node.cpp src/GENOME/link.cpp src/MATH/ranges.cpp src/MATH/functions.cpp src/SPECIATION/speciation.cpp src/SPECIATION/crossover.cpp
EXEC = program

all: $(EXEC)

$(EXEC): $(SOURCES)
	$(CC) -o $@ $^

clean:
	rm -f $(EXEC)
