#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>


typedef struct treeNode
   { 
    struct treeNode * childL;
    struct treeNode * childM;
    struct treeNode * childR;
    struct treeNode * sibling;
    int lineno;
    int token;
    int tipo;
    char * lexema;
   } TreeNode; // no da arvore

typedef struct no
{
    int linhas;
    char * lexema;
    char * escopo;
    int tipo;
    int reg;
    struct no * proximo;
}Tno; //no da tabela hash

typedef struct lista
{
    Tno * inicio;
    int tam;
}Tlista; //linha da tabela hash

typedef struct inst
{
    char * instrucao;
    char * op1;
    char * op2;
    char * res;
    struct inst * proximo;
}Tinst; //no que representa quadrupla

typedef struct quadruplas
{
    Tinst * inicio;
    int tam;
}Tquadruplas; //lista de quadruplas

extern FILE * fonte;
extern FILE * arvore;
extern FILE * tokens;
extern FILE * intercode;
extern int lineno;
extern char * lexema;
extern TreeNode * savedTree;
extern char * escopo;
extern Tlista * tabela[211];
extern FILE * saidaTabela;
//extern static TreeNode * savedTree;

#endif