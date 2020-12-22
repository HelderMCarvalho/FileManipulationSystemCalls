all: ./objects/main.o
	gcc -o TP ./objects/main.o -lm

./objects/main.o: main.c main.h
	mkdir -p objects
	gcc -c -lm main.c -o ./objects/main.o