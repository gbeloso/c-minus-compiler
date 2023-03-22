#ifndef _UTI_H_
#define _UTIL_H_

#include <string.h>
#include <stdlib.h>
#include "globals.h"

char *  copiaString(char * palavra);
TreeNode * newNode(int token, char * lexema, int tipo, int linha); // funcao para criar um novo no da arvore sintatica
void destroiNo(TreeNode * t); // funcao para destruir um no da arvore
typedef enum {VarNode, VetorNode, OpNode, NumNode, AtivNode, ReturnNode, WhileNode, IfNode, Decl_varNode, Decl_vetorNode, FunNode, ParamNode, VetorParamNode};
void printTree(TreeNode * t, int n);
void destroi(TreeNode * t);
#endif