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

extern FILE * fonte;
extern FILE * arvore;
extern FILE * tokens;
extern int lineno;
extern char * lexema;
extern TreeNode * savedTree;
//extern static TreeNode * savedTree;

#endif