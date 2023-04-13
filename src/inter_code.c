#include "globals.h"
#include "parser.tab.h"
#include "util.h"
#include "inter_code.h"
#include <stdlib.h>

int cont = 0;
FILE * intercode;

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
		fprintf(intercode, "%d | %s, %s, %s, %s\n", i, aux->instrucao, aux->op1, aux->op2, aux->res);
		aux = aux->proximo;
		i++;
	}
} 

char * generateInterCode(TreeNode * t, Tquadruplas * quadrupla){
	int reg_e, reg_m;
	if(t == NULL){
		return -1;
	}
	else{
		if(t->tipo == OpNode){
			if(t->childL->tipo != NumNode){
				reg_e = generateInterCode(t->childL, quadrupla);
			}
			if(t->childM->tipo != NumNode){
				reg_m = generateInterCode(t->childM, quadrupla);
			}
			if(t->token == PLUS){
				insere_inst("ADD", "R?", "R?", "R?", quadrupla);
			}
			else if(t->token ==  MINUS){
				insere_inst("SUB", "R?", "R?", "R?", quadrupla);
			}
			else if(t->token == TIMES){
				insere_inst("MULT", "R?", "R?", "R?", quadrupla);
			}
			else if(t->token == SLASH){
				insere_inst("DIV", "R?", "R?", "R?", quadrupla);
			}
		}
		else if(t->tipo == NumNode){
			return t->lexema;
		}
		else if(t->tipo == VarNode){
			int aux;
			aux = cont;
			cont++;
			char * temp = concat("R", itoa(aux));
			return ;
		}
		if(t->tipo != OpNode){
			generateInterCode(t->childL, quadrupla);
			generateInterCode(t->childM, quadrupla);
			generateInterCode(t->childR, quadrupla);
		}
		generateInterCode(t->sibling, quadrupla);
		return;
	}
}

char * concat(char * str0, char * str1){
	
}