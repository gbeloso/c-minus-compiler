#include "globals.h"
#include "scan.h"
#include "util.h"
#include "parser.tab.h"


void main(int arcg, char ** argv){
    int temp;
    fonte = fopen(argv[1], "r");
    tokens = fopen("saida/tokens.txt", "w");
    arvore = fopen("saida/arvore.txt", "w");
    yyin = fonte;
    while(temp){
        temp = yylex();
    }
}