build:
	gcc -Wall -std=c99 ./src/*c -o game

run:
	./game

clear:
	rm game