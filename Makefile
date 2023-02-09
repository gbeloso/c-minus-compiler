

all:
	flex --header-file=src/scan.h --outfile=src/scan.c src/scan.l
	gcc src/scan.c -o scan.o

run:
	./scan.o exemplos/exemplo.c