#include "globals.h"
#include "scan.h"
#include "util.h"
#include "analyze.h"
#include "symtab.h"
#include "parser.tab.h"
#include "inter_code.h"

void main(int arcg, char ** argv){
    int temp = 1;
    int i;
    fonte = fopen("exemplos/exemplo.c", "r");
    tokens = fopen("saida/tokens.txt", "w");
    arvore = fopen("saida/arvore.txt", "w");
    saidaTabela = fopen("saida/tabela.txt", "w");
    yyin = fonte;
    yyparse();
    if (savedTree != NULL){
        printTree(savedTree, 0);
    }
    escopo = "global";
    for (i = 0; i < 211; i++){
        tabela[i] = criaLista();
    }
    temp = hash("input", "global");
    insere(0, "input", "global", INT, FunNode, tabela[temp]);
    temp = hash("output", "global");
    insere(0, "output", "global", VOID, FunNode, tabela[temp]);
    analyze(savedTree);
    temp = hash("main", "global");
    if(buscar("main", "global", FunNode, tabela[temp]->inicio) == NULL){
        semantic_error(0, "main");
    }
    for (i = 0; i < 211; i++){
        if(tabela[i]->inicio != NULL){
            fprintf(saidaTabela, "tabela[%d]: ", i);
            imprime(tabela[i]);
            fprintf(saidaTabela, "\n");   
        }
    }
    intercode = fopen("saida/inter_code.txt", "w");
    Tquadruplas * quadrupla;
    quadrupla = criaQuadrupla();
    generateInterCode(savedTree, quadrupla);
    imprimeQuadruplas(quadrupla);
}