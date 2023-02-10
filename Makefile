

all:
	flex --header-file=src/scan.h --outfile=src/scan.c src/scan.l
	gcc -c src/util.c -o obj/util.o
	gcc  src/scan.c obj/util.o -o obj/scan.o

run:
	./obj/scan.o exemplos/exemplo.c