#include "globals.h"
#include "parser.tab.h"
#include "util.h"
#include "inter_code.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_REGS 58

int lineCounter = 0;
int cont_reg = 0;
int cont_label = 0;
char * endLabel;
FILE * intercode;
char * func_atual = "global";

Tregs * registradores;

Tregs * criaRegs(){
	Tregs * regs;
	regs = (Tregs*)malloc(sizeof(Tregs));
	regs->inicio = NULL;
	regs->fim = NULL;
	regs->tam = 0;
	return regs;
}

Tquadruplas * criaQuadrupla(){
	Tquadruplas * quadrupla;
	quadrupla = (Tquadruplas * )malloc(sizeof(Tquadruplas));
	quadrupla->inicio = NULL;
	quadrupla->tam = 0;
	return quadrupla;
}

void insere_inst(char * instrucao, char * op1, char * op2, char * res, int arrayType, Tquadruplas * quadrupla){
	Tinst * inst, * aux;
	inst = (Tinst *) malloc(sizeof(Tinst));
	inst->instrucao = copiaString(instrucao);
	inst->op1 = copiaString(op1);
	inst->op2 = copiaString(op2);
	inst->res = copiaString(res);
	inst->line = lineCounter;
	inst->arrayType = arrayType;
	inst->proximo = NULL;
	lineCounter++;
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

void imprimeQuadruplas(Tquadruplas * quadrupla){
	Tinst * aux;
	aux = quadrupla->inicio;
	while(aux != NULL){
		fprintf(intercode, "%d: (%s, %s, %s, %s)", aux->line, aux->instrucao, aux->op1, aux->op2, aux->res);
		if(aux->arrayType == 1){
			fprintf(intercode, " vetor? %d\n", aux->arrayType);
		}
		else{
			fprintf(intercode, "\n");
		}
		aux = aux->proximo;
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
			endLabel = criaLabel();
			func_atual = copiaString(t->lexema);
			if(t->token == INT){
				op1 = copiaString("INT");
			}
			else{
				op1 = copiaString("VOID");
			}
			insere_inst("FUN", op1, t->lexema, "-", 0, quadrupla);
			aux = t->childL;
			while(aux != NULL){
				generateInterCode(aux, quadrupla);
				aux = aux->sibling;
			}
			aux = t->childM;
			while(aux != NULL){
				generateInterCode(aux, quadrupla);
				aux = aux->sibling;
			}
			func_atual = copiaString("global");
			insere_inst("LAB", endLabel, "-", "-", 0, quadrupla);
			insere_inst("END", t->lexema, "-", "-", 0, quadrupla);
			generateInterCode(t->sibling, quadrupla);
			if(t->sibling == NULL){
				insere_inst("HALT", "-", "-", "-", 0, quadrupla);
			}
		}
		else if(t->tipo == ParamNode){
			insere_inst("ARG", "INT", t->lexema, func_atual, 0, quadrupla);
		}
		else if(t->tipo == VetorParamNode){
			insere_inst("ARG", "INT", t->lexema, func_atual, 1, quadrupla);
		}
		else if(t->tipo == IfNode){
			TreeNode * aux;
			if(t->childR != NULL){
				op1 = generateInterCode(t->childL, quadrupla);
				op2 = criaLabel();
				op3 = criaLabel();
				insere_inst("IFF", op1, op2, "-", 0, quadrupla);
				aux = t->childM;
				while(aux != NULL){
					generateInterCode(aux, quadrupla);
					aux = aux->sibling;
				}
				insere_inst("GOTO", op3, "-", "-", 0, quadrupla);
				insere_inst("LAB", op2, "-", "-", 0, quadrupla);
				aux = t->childR;
				while(aux != NULL){
					generateInterCode(aux, quadrupla);
					aux = aux->sibling;
				}
				insere_inst("LAB", op3, "-", "-", 0, quadrupla);
			}
			else{
				op1 = generateInterCode(t->childL, quadrupla);
				op2 = criaLabel();
				insere_inst("IFF", op1, op2, "-", 0, quadrupla);
				aux = t->childM;
				while(aux != NULL){
					generateInterCode(aux, quadrupla);
					aux = aux->sibling;
				}
				insere_inst("LAB", op2, "-", "-", 0, quadrupla);
			}
			
		}
		else if(t->tipo == OpNode){
			if(t->token == EQUAL){
				if(t->childL->tipo == VarNode){
					op1 = atribuiReg(t->childL->lexema, func_atual);
					op2 = generateInterCode(t->childM, quadrupla);
					insere_inst("ASSIGN", op1, op2, "-", 0, quadrupla);
					insere_inst("STORE", op1, t->childL->lexema, "-", 0, quadrupla);
				}
				else if(t->childL->tipo == VetorNode){
					op1 = atribuiReg(NULL, NULL);
					op2 = generateInterCode(t->childM, quadrupla);
					insere_inst("ASSIGN", op1, op2, "-", 0, quadrupla);
					op3 = generateInterCode(t->childL->childL, quadrupla);
					insere_inst("STORE", op1, t->childL->lexema, op3, 0, quadrupla);
				}
				return op1;
			}
			else{
				op1 = generateInterCode(t->childL, quadrupla);
				op2 = generateInterCode(t->childM, quadrupla);
				op3 = atribuiReg(NULL, NULL);
				if(t->token == DEQUAL){
					insere_inst("EQUAL", op1, op2, op3, 0, quadrupla);
				}
				else if(t->token == PLUS){
					insere_inst("ADD", op1, op2, op3, 0, quadrupla);	
				}
				else if(t->token == MINUS){
					insere_inst("SUB", op1, op2, op3, 0, quadrupla);		
				}
				else if(t->token == TIMES){
					insere_inst("MULT", op1, op2, op3, 0, quadrupla);		
				}
				else if(t->token == SLASH){
					insere_inst("DIV", op1, op2, op3, 0, quadrupla);		
				}
				else if(t->token == LT){
					insere_inst("SLT", op1, op2, op3, 0, quadrupla);	
				}
				else if(t->token == GT){
					insere_inst("SGT", op1, op2, op3, 0, quadrupla);	
				}
				else if(t->token == LE){
					insere_inst("SLE", op1, op2, op3, 0, quadrupla);	
				}
				else if(t->token == GE){
					insere_inst("SGE", op1, op2, op3, 0, quadrupla);	
				}
				else if(t->token == DIFFERENT){
					insere_inst("SDT", op1, op2, op3, 0, quadrupla);	
				}
				return op3;
			}
		}
		else if(t->tipo == NumNode){
			return(t->lexema);
		}
		else if(t->tipo == VarNode){
			op1 = atribuiReg(t->lexema, func_atual);
			insere_inst("LOAD", op1, t->lexema, "-", 0, quadrupla);
			return op1;	
		}
		else if(t->tipo == ReturnNode){
			op1 = generateInterCode(t->childL, quadrupla);
			insere_inst("RET", op1, "-", "-", 0, quadrupla);
			insere_inst("GOTO", endLabel, "-", "-", 0, quadrupla);
			return op1;		
		}
		else if(t->tipo == AtivNode){
			if(strcmp(t->lexema, "output") == 0){
				op1 = generateInterCode(t->childL, quadrupla);
				insere_inst("OUT", op1, "-", "-", 0, quadrupla);
			}
			else if(strcmp(t->lexema, "input") == 0){
				op1 = atribuiReg(NULL, NULL);
				insere_inst("IN", op1, "-", "-", 0, quadrupla);
				return op1;
			}
			else{
				TreeNode * aux;
				int cont = 0;
				aux = t->childL;
				while(aux != NULL){
					op1 = generateInterCode(aux, quadrupla);
					insere_inst("PARAM", op1, "-", "-", 0, quadrupla);
					aux = aux->sibling;
					cont++;
				}
				op2 = (char * ) malloc(sizeof(char)*(int)log10(cont));
				sprintf(op2, "%d", cont);
				op3 = atribuiReg(NULL, NULL);
				insere_inst("CALL", t->lexema, op2, op3, 0, quadrupla);
				return(op3);	
			}	
		}
		else if(t->tipo == Decl_varNode){
			insere_inst("ALLOC", t->lexema, func_atual, "-", 0, quadrupla);
			if(strcmp(func_atual, "global") == 0){
				generateInterCode(t->sibling, quadrupla);
			}
		}
		else if(t->tipo == Decl_vetorNode){
			op3 = generateInterCode(t->childL, quadrupla);
			insere_inst("ALLOC", t->lexema, func_atual, op3, 1, quadrupla);
			if(strcmp(func_atual, "global") == 0){
				generateInterCode(t->sibling, quadrupla);
			}
		}
		else if(t->tipo == VetorNode){
			op1 = atribuiReg(NULL, NULL);
			op2 = generateInterCode(t->childL, quadrupla);
			//op3 = atribuiReg();
			//insere_inst("MULT", op2, "4", op2, quadrupla);
			insere_inst("LOAD", op1, t->lexema, op2, 0, quadrupla);
			return op1;
		}
		else if(t->tipo == WhileNode){
			TreeNode * aux;
			op2 = criaLabel();
			op3 = criaLabel();
			insere_inst("LAB", op2, "-", "-", 0, quadrupla);
			op1 = generateInterCode(t->childL, quadrupla);
			insere_inst("IFF", op1, op3, "-", 0, quadrupla);
			aux = t->childM;
			while(aux != NULL){
				generateInterCode(aux, quadrupla);
				aux = aux->sibling;
			}
			insere_inst("GOTO", op2, "-", "-", 0, quadrupla);
			insere_inst("LAB", op3, "-", "-", 0, quadrupla);
		}
		else{
			return NULL;
		}
	}
	return NULL;
}

char * criaLabel(){
	char * temp;
	temp = (char *) malloc(10*(sizeof(char)));
	sprintf(temp, "L%d", cont_label);
	cont_label++;
	return temp;
}

void insere_reg(char * var, char * escopo, int reg){
	Tcelula * registrador;
	registrador = (Tcelula*)malloc(sizeof(Tcelula));
	registrador->var = copiaString(var);
	registrador->escopo = copiaString(escopo);
	registrador->reg = reg;
	if(registradores->inicio == NULL){
		registradores->inicio = registrador;
		registradores->fim = registrador;
		registradores->tam = registradores->tam + 1;
	}
	else{
		registradores->fim->proximo = registrador;
		registrador->proximo = NULL;
		registradores->fim = registrador;
		registradores->tam = registradores->tam + 1;
	}
}

int busca_reg(char * var, char * escopo){
	if(registradores->inicio == NULL || var == NULL){
		return -1;
	}
	else{
		Tcelula * aux;
		aux = registradores->inicio;
		while(aux != NULL){
			if(aux->var == NULL){
				aux = aux->proximo;
			}
			else{
				if(strcmp(var, aux->var) == 0 && strcmp(escopo, aux->escopo) == 0){
					return(aux->reg);
				}
				aux = aux->proximo;
			}
		}
		return -1;
	}
}


char * atribuiReg(char * var, char * escopo){
	char * temp;
	int i;
	temp = (char *)malloc(sizeof(char)*3);
	int len = sizeof(temp);
	int reg = busca_reg(var, escopo);
	if(reg == -1){
		for(i = 0; i < MAX_REGS;i++){
			if(reg_is_free(i)){
				insere_reg(var, escopo, i);
				snprintf(temp, len, "R%d", i);
				return(temp);	
			}
		}		
	}
	else{
		sprintf(temp, "R%d", reg);
		return(temp);
	}
	return NULL;
}

int reg_is_free(int reg){
	Tcelula * aux;
	aux = registradores->inicio;
	while(aux != NULL){
		if(aux->reg == reg){
			return(0);
		}
		aux = aux->proximo;
	}
	return(1);
}

void limpa_regs(void){
	while(registradores->inicio != NULL){
		remove_reg(registradores->inicio);
	}
}

void remove_reg(Tcelula * registro){
	Tcelula * aux;
	Tcelula * anterior;
	anterior = NULL;
	aux = registradores->inicio;
	while(aux != registro){
		anterior = aux;
		aux = aux->proximo;
	}
	if(anterior == NULL){
		registradores->inicio = aux->proximo;
		aux->proximo = NULL;
		free(aux);
	}
	else{
		anterior = aux->proximo;
		free(aux);
	}
}

// analise semantica vetor arrumar