CC = gcc
CFLGAS = -std=c11 -Werror -Wall -g
OBJ = bin/algorithms.o bin/dataExtract.o bin/scheduler.o
EXEC = scheduler

$(EXEC): $(OBJ)
	$(CC) $(OBJ) $(CFLGAS) -o $(EXEC)

bin/scheduler.o: src/scheduler.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

bin/algorithms.o: src/algorithms.c src/algorithms.h
	@mkdir -p bin
	$(CC) $(CFLGAS) -c $< -o $@

bin/dataExtract.o: src/dataExtract.c src/dataExtract.h
	@mkdir -p bin
	$(CC) $(CFLGAS) -c $< -o $@

clean:
	rm -rf bin scheduler


