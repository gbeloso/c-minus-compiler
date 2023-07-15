#include "util.h"
#include "globals.h"
#include "parser.tab.h"

char * copiaString(char * palavra){
    int n;
    char * t;
    if (palavra == NULL) return NULL;
    n = strlen(palavra)+1;
    t = malloc(n);
    if (t == NULL)
        printf("Out of memory error at line %d\n",lineno);
    else strcpy(t,palavra);
    return t;
}

TreeNode * newNode(int token, char * lexema, int tipo, int linha)
{ 
    TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    if (t==NULL)
        printf("Out of memory error at line %d\n",linha);
    else {
        t->childL = NULL;
        t->childM = NULL;
        t->childR = NULL;
        t->sibling = NULL;
        t->token = token;
        t->lineno = linha;
        t->tipo = tipo;
        t->lexema = copiaString(lexema);
    }
    return t;
}

void printTree(TreeNode * t, int n){ //funcao para printar a arvore em modo identado
    if(t == NULL){
        return;
    }
    else{
        for (unsigned int i = 0; i < n; i++)
            fprintf(arvore, "\t");

        if(t->tipo == NumNode){
           fprintf(arvore, "Num: ");
        }
        else if(t->tipo == VarNode){
            fprintf(arvore, "Var: ");
        }
        else if(t->tipo == OpNode){
           fprintf(arvore, "Op: ");
        }
        else if(t->tipo == AtivNode){
           fprintf(arvore, "Ativacao: ");
        }
        else if(t->tipo == Decl_varNode){
            fprintf(arvore, "Decl_var: ");
        }
        else if(t->tipo == Decl_vetorNode){
            fprintf(arvore, "Decl_Vetor: ");
        }
        else if(t->tipo == FunNode){
            fprintf(arvore, "Func: ");
        }
        else if(t->tipo == ParamNode){
            fprintf(arvore, "Param: ");
        }
        else if(t->tipo == VetorParamNode){
            fprintf(arvore, "ParamVetor: ");
        }
        else if(t->tipo == VetorNode){
            fprintf(arvore, "Vetor: ");
        }
        fprintf(arvore, "%s ", t->lexema);
        if(t->token == INT){
           fprintf(arvore, "int\n");
        }
        else if(t->token == VOID){
            fprintf(arvore, "void\n");
        }
        else{
            fprintf(arvore, "\n");
        }
        if(t->childL != NULL)
            printTree(t->childL, n+1);
        if(t->childM != NULL)
            printTree(t->childM, n+1);
        if(t->childR != NULL)
            printTree(t->childR, n+1);
        if(t->sibling != NULL){
            printTree(t->sibling, n);
        }
    }
}

void destroi(TreeNode * t){ // funcao para destruir a arvore, percorre ela em modo pos ordem apra destruir de baixo para cima
    if(t == NULL){
        return;
    }
    else{
        if(t->sibling != NULL){
            destroi(t->sibling);
        }
        destroi(t->childL);
        destroi(t->childM);
        destroi(t->childR);
        destroiNo(t);
    }
}

void destroiNo(TreeNode * t){ //funcao para destruir alocacoes feitas em um no
    free(t->lexema);
    free(t);
}