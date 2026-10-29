#include "globals.h"
#include "scan.h"
#include "util.h"
#include "analyze.h"
#include "symtab.h"
#include "parser.tab.h"
#include "inter_code.h"
#include "assembly.h"
#include "mem.h"
#include "binary.h"
#include <stdlib.h>
#include <string.h>

char * diretorioSaida;

/* Monta um caminho "<diretorioSaida>/<arquivo>" em um buffer recem-alocado. */
char * caminhoSaida(char * arquivo){
    int tam = strlen(diretorioSaida) + strlen(arquivo) + 2;
    char * caminho = (char *) malloc(sizeof(char) * tam);
    sprintf(caminho, "%s/%s", diretorioSaida, arquivo);
    return caminho;
}

int main(int argc, char ** argv){
    int temp = 1;
    int i;
    char * arquivoEntrada = "exemplos/exemplo.c";
    diretorioSaida = "saida";
    if(argc >= 2){
        arquivoEntrada = argv[1];
    }
    if(argc >= 3){
        diretorioSaida = argv[2];
    }
    fonte = fopen(arquivoEntrada, "r");
    tokens = fopen(caminhoSaida("tokens.txt"), "w");
    arvore = fopen(caminhoSaida("arvore.txt"), "w");
    saidaTabela = fopen(caminhoSaida("tabela.txt"), "w");
    saidaAssembly = fopen(caminhoSaida("assembly.txt"), "w");
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
    intercode = fopen(caminhoSaida("inter_code.txt"), "w");
    Tquadruplas * quadrupla;
    quadrupla = criaQuadrupla();
    registradores = criaRegs();
    generateInterCode(savedTree, quadrupla);
    imprimeQuadruplas(quadrupla);
    TAssemblyCode * assemblyList = criaAssemblyCode();
    memoriaLista = criaListaMemoria();
    geraAssembly(quadrupla, assemblyList);
    imprimeAssembly(assemblyList);
    imprimeMemoriaLista(memoriaLista);
    TBinCode * binCodeList = criaBinCode();
    geraBinCode(binCodeList, assemblyList);
    imprimeBinCode(binCodeList);
    return 0;
}