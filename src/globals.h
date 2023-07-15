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
    int tipo_id;
    int tipo_node;
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
    int line;
    int arrayType;
    struct inst * proximo;
}Tinst; //no que representa quadrupla

typedef struct quadruplas
{
    Tinst * inicio;
    int tam;
}Tquadruplas; //lista de quadruplas

typedef struct celula
{
    char * var;
    char * escopo;
    int reg;
    struct celula * proximo;
}Tcelula;

typedef struct regs
{
    Tcelula * inicio;
    Tcelula * fim;
    int tam;
}Tregs;

typedef struct AssemblyInst
{
    char * instrucao;
    char * rd;
    char * rm;
    char * rn;
    char * imm;
    int line;
    struct AssemblyInst * proximo;
}TAssemblyInst; //no que representa instrucao assembly

typedef struct AssemblyCode
{
    int tam;
    TAssemblyInst * inicio;
}TAssemblyCode; //lista de intrucoes assembly

typedef struct Data{
    char * dataName;
    int size;
    int memoryPos;
    int dataType;
    int array;
    struct Data * proximo;
} TData;

typedef struct Memory{
    char * escopo;
    int tamEscopo;
    TData * dataList;
    struct Memory * proximo;
} TMemory;

typedef struct  MemoryList{
    TMemory * inicio;
    TMemory * fim;
} TMemoryList;


typedef struct BinInst
{
    char * instr;
    int line;
    struct BinInst * proximo;
}TBinInst; //no que representa instrucao binaria

typedef struct BinCode
{
    int tam;
    TBinInst * inicio;
    TBinInst * fim;
}TBinCode; //lista de intrucoes binarias

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
extern FILE * saidaAssembly;
extern Tregs * registradores;
extern TMemoryList * memoriaLista;
//extern static TreeNode * savedTree;

#endif