CC = gcc
STD = c99

build:
	gcc -Wall -std=c99 ./src/*.c -o renderer -I/opt/homebrew/include/ -L/opt/homebrew/lib -lSDL3

run:
	./renderer

buildrun:
	gcc -Wall -std=c99 ./src/*.c -o renderer -I/opt/homebrew/include/ -L/opt/homebrew/lib -lSDL3 run

clean:
	rm renderer
