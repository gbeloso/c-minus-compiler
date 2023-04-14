#include "globals.h"
#include "parser.tab.h"
#include "util.h"
#include "inter_code.h"
#include <stdlib.h>
#include <string.h>

int cont_reg = 0;
int cont_label = 0;
FILE * intercode;
char * func_atual = "global";

Tquadruplas * criaQuadrupla(){
	Tquadruplas * quadrupla;
	quadrupla = (Tquadruplas * )malloc(sizeof(Tquadruplas));
	quadrupla->inicio = NULL;
	quadrupla->tam = 0;
	return quadrupla;
}

void insere_inst(char * instrucao, char * op1, char * op2, char * res, Tquadruplas * quadrupla){
	Tinst * inst, * aux;
	inst = (Tinst *) malloc(sizeof(Tinst));
	inst->instrucao = copiaString(instrucao);
	inst->op1 = copiaString(op1);
	inst->op2 = copiaString(op2);
	inst->res = copiaString(res);
	if(quadrupla->inicio == NULL){
		quadrupla->inicio = inst;
		quadrupla->tam = quadrupla->tam + 1;
	}
	else{
		aux = quadrupla->inicio;
		while(aux->proximo != NULL){
			aux = aux->proximo;
		}
		aux->proximo = inst;
		quadrupla->tam = quadrupla->tam + 1;
	}
}

void deleta_inst(Tinst * inst, Tquadruplas * quadrupla){

}

Tno * busca_op(char * op, Tinst * inicio){

}

void imprimeQuadruplas(Tquadruplas * quadrupla){
	Tinst * aux;
	int i = 1;
	aux = quadrupla->inicio;
	while(aux != NULL){
		fprintf(intercode, "%d | (%s, %s, %s, %s)\n", i, aux->instrucao, aux->op1, aux->op2, aux->res);
		aux = aux->proximo;
		i++;
	}
} 

char * generateInterCode(TreeNode * t, Tquadruplas * quadrupla){
	char * op1, * op2, * op3; 
	if(t == NULL){
		return NULL;
	}
	else{
		if(t->tipo == FunNode){
			TreeNode * aux;
			func_atual = copiaString(t->lexema);
			if(t->token == INT){
				op1 = copiaString("INT");
			}
			else{
				op1 = copiaString("VOID");
			}
			insere_inst("FUN", op1, t->lexema, "-", quadrupla);
			generateInterCode(t->childL, quadrupla);
			aux = t->childM;
			while(aux != NULL){
				generateInterCode(aux, quadrupla);
				aux = aux->sibling;
			}
			func_atual = copiaString("global");
			insere_inst("END", t->lexema, "-", "-", quadrupla);
			generateInterCode(t->sibling, quadrupla);
			if(t->sibling == NULL){
				insere_inst("HALT", "-", "-", "-", quadrupla);
			}
		}
		else if(t->tipo == ParamNode){
			insere_inst("ARG", "INT", t->lexema, func_atual, quadrupla);
			generateInterCode(t->sibling, quadrupla);
			op1 = (char * ) malloc(sizeof(char)*10);
			sprintf(op1, "R%d", cont_reg);
			cont_reg++;
			insere_inst("LOAD", op1, t->lexema, "-", quadrupla);
		}
		else if(t->tipo == IfNode){
			op1 = generateInterCode(t->childL, quadrupla);
			op2 = (char * ) malloc(sizeof(char)*10);
			sprintf(op2, "L%d", cont_label);
			cont_label++;
			insere_inst("IFF", op1, op2, "-", quadrupla);
			op3 = generateInterCode(t->childM, quadrupla);
			sprintf(op3, "L%d", cont_label);
			cont_label++;
			insere_inst("GOTO", op3, "-", "-", quadrupla);
			insere_inst("LAB", op2, "-", "-", quadrupla);
			op1 = generateInterCode(t->childR, quadrupla);
			insere_inst("GOTO", op3, "-", "-", quadrupla);
			insere_inst("LAB", op3, "-", "-", quadrupla);
		}
		else if(t->tipo == OpNode){
			op1 = generateInterCode(t->childL, quadrupla);
			op2 = generateInterCode(t->childM, quadrupla);
			if(t->token == DEQUAL){
				op3 = (char * ) malloc(sizeof(char)*10);
				sprintf(op3, "R%d", cont_reg);
				cont_reg++;
				insere_inst("EQUAL", op1, op2, op3, quadrupla);
			}
			else if(t->token == EQUAL){
				insere_inst("ASSIGN", op1, op2, "-", quadrupla);
				insere_inst("STORE", t->childL->lexema, op1, "-", quadrupla);
				return op1;
			}
			else if(t->token == PLUS){
				op3 = (char * ) malloc(sizeof(char)*10);
				sprintf(op3, "R%d", cont_reg);
				cont_reg++;
				insere_inst("ADD", op1, op2, op3, quadrupla);	
			}
			else if(t->token == MINUS){
				op3 = (char * ) malloc(sizeof(char)*10);
				sprintf(op3, "R%d", cont_reg);
				cont_reg++;
				insere_inst("SUB", op1, op2, op3, quadrupla);		
			}
			else if(t->token == TIMES){
				op3 = (char * ) malloc(sizeof(char)*10);
				sprintf(op3, "R%d", cont_reg);
				cont_reg++;
				insere_inst("MULT", op1, op2, op3, quadrupla);		
			}
			else if(t->token == SLASH){
				op3 = (char * ) malloc(sizeof(char)*10);
				sprintf(op3, "R%d", cont_reg);
				cont_reg++;
				insere_inst("DIV", op1, op2, op3, quadrupla);		
			}
			return op3;
		}
		else if(t->tipo == NumNode){
			return(t->lexema);
		}
		else if(t->tipo == VarNode){
			op1 = (char * ) malloc(sizeof(char)*10);
			sprintf(op1, "R%d", cont_reg);
			cont_reg++;
			insere_inst("LOAD", op1, t->lexema, "-", quadrupla);
			return op1;	
		}
		else if(t->tipo == ReturnNode){
			op1 = generateInterCode(t->childL, quadrupla);
			insere_inst("RET", op1, "-", "-", quadrupla);
			return op1;		
		}
		else if(t->tipo == AtivNode){
			TreeNode * aux;
			int cont = 0;
			aux = t->childL;
			while(aux != NULL){
				op1 = generateInterCode(aux, quadrupla);
				insere_inst("PARAM", op1, "-", "-", quadrupla);
				aux = aux->sibling;
				cont++;
			}
			op2 = (char * ) malloc(sizeof(char)*10);
			sprintf(op2, "%d", cont);
			op3 = (char * ) malloc(sizeof(char)*10);
			sprintf(op3, "R%d", cont_reg);
			cont_reg++;
			insere_inst("CALL", t->lexema, op2, op3, quadrupla);
			return(op3);
		}
		else if(t->tipo == Decl_varNode){
			insere_inst("ALLOC", t->lexema, func_atual, "-", quadrupla);
		}
	}
}