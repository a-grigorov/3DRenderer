CC = gcc
STD = c99

build:
	${CC} -Wall -std=${STD} ./src/*.c -o renderer -I/opt/homebrew/include/ -L/opt/homebrew/lib -lSDL3

run:
	./renderer

buildrun:
	${CC} -Wall -std=${STD} ./src/*.c -o renderer -I/opt/homebrew/include/ -L/opt/homebrew/lib -lSDL3
	./renderer

clean:
	rm renderer
