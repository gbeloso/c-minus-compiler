

all:
	flex --header-file=src/scan.h --outfile=src/scan.c src/scan.l
	bison -d src/parser.y -o src/parser.tab.c
	gcc -Wall src/main.c src/scan.c src/util.c src/parser.tab.c src/analyze.c src/symtab.c src/inter_code.c src/assembly.c src/mem.c src/binary.c -o obj/main.o -g -lm

run:
	./obj/main.o exemplos/exemplo.c

debug:
	gdb ./obj/main.o exemplos/exemplo.c