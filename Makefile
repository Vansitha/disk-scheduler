CC = gcc
CFLGAS = -std=c11 -pthread -Wall -g
OBJ = bin/algorithms.o bin/dataExtract.o bin/simulator.o
EXEC = simulator 

$(EXEC): $(OBJ)
	$(CC) $(OBJ) $(CFLGAS) -o $(EXEC)

bin/simulator.o: src/simulator.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

bin/algorithms.o: src/algorithms.c src/algorithms.h
	@mkdir -p bin
	$(CC) $(CFLGAS) -c $< -o $@

bin/dataExtract.o: src/dataExtract.c src/dataExtract.h
	@mkdir -p bin
	$(CC) $(CFLGAS) -c $< -o $@

clean:
	rm -rf bin simulator 


