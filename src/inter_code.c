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

Tregs * criaRegs(){
	Tregs * regs;
	regs = (Tregs*)malloc(sizeof(Tregs));
	regs->inicio = NULL;
	regs->tam = 0;
	return regs;
}

Tregs * registradores;

Tquadruplas * criaQuadrupla(){
	Tquadruplas * quadrupla;
	quadrupla = (Tquadruplas * )malloc(sizeof(Tquadruplas));
	quadrupla->inicio = NULL;
	quadrupla->tam = 0;
	return quadrupla;
}

void insere_reg(char * var, char * escopo, int reg){
	Tcelula * registrador;
	Tcelula * aux;
	registrador = (Tcelula*)malloc(sizeof(Tcelula));
	registrador->var = copiaString(var);
	registrador->escopo = copiaString(escopo);
	registrador->reg = reg;
	if(registradores->inicio == NULL){
		registradores->inicio = registrador;
		registradores->tam = registradores->tam + 1;
	}
	else{
		aux = registradores->inicio;
		while(aux->proximo != NULL){
			aux = aux->proximo;
		}
		aux->proximo = registrador;
		registradores->tam = registradores->tam + 1;
	}
}

int busca_reg(char * var, char * escopo){
	if(registradores->inicio == NULL){
		return -1;
	}
	else{
		Tcelula * aux;
		aux = registradores->inicio;
		while(aux->proximo != NULL){
			int t0 = strcmp(var, aux->var);
			int t1 = strcmp(escopo, aux->escopo);
			if(t0 == 0 && t1 == 0){
				return(aux->reg);
			}
		}
		return -1;
	}
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

void imprimeQuadruplas(Tquadruplas * quadrupla){
	Tinst * aux;
	aux = quadrupla->inicio;
	while(aux != NULL){
		fprintf(intercode, "(%s, %s, %s, %s)\n", aux->instrucao, aux->op1, aux->op2, aux->res);
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
		else if(t->tipo == ParamNode || t->tipo == VetorParamNode){
			insere_inst("ARG", "INT", t->lexema, func_atual, quadrupla);
			generateInterCode(t->sibling, quadrupla);
			op1 = atribuiReg(t->lexema, func_atual);
			insere_inst("LOAD", op1, t->lexema, "-", quadrupla);
		}
		else if(t->tipo == IfNode){
			TreeNode * aux;
			if(t->childR != NULL){
				op1 = generateInterCode(t->childL, quadrupla);
				op2 = criaLabel();
				op3 = criaLabel();
				insere_inst("IFF", op1, op2, "-", quadrupla);
				aux = t->childM;
				while(aux != NULL){
					generateInterCode(aux, quadrupla);
					aux = aux->sibling;
				}
				insere_inst("GOTO", op3, "-", "-", quadrupla);
				insere_inst("LAB", op2, "-", "-", quadrupla);
				aux = t->childR;
				while(aux != NULL){
					generateInterCode(aux, quadrupla);
					aux = aux->sibling;
				}
				insere_inst("LAB", op3, "-", "-", quadrupla);
			}
			else{
				op1 = generateInterCode(t->childL, quadrupla);
				op2 = criaLabel();
				insere_inst("IFF", op1, op2, "-", quadrupla);
				aux = t->childM;
				while(aux != NULL){
					generateInterCode(aux, quadrupla);
					aux = aux->sibling;
				}
				generateInterCode(t->childM, quadrupla);
				insere_inst("LAB", op2, "-", "-", quadrupla);
			}
			
		}
		else if(t->tipo == OpNode){
			op1 = generateInterCode(t->childL, quadrupla);
			op2 = generateInterCode(t->childM, quadrupla);
			if(t->token == DEQUAL){
				op3 = atribuiReg();
				insere_inst("EQUAL", op1, op2, op3, quadrupla);
			}
			else if(t->token == EQUAL){
				insere_inst("ASSIGN", op1, op2, "-", quadrupla);
				if(t->childL->tipo == VarNode){
					insere_inst("STORE", op1, t->childL->lexema, "-", quadrupla);
				}
				else if(t->childL->tipo == VetorNode){
					insere_inst("ASSIGN", op1, op2, "-", quadrupla);
					op3 = generateInterCode(t->childL->childL, quadrupla);
					op2 = atribuiReg();
					insere_inst("MULT", op3, "4", op2, quadrupla);
					insere_inst("STORE", op1, t->childL->lexema, op2, quadrupla);
				}
				return op1;
			}
			else if(t->token == PLUS){
				op3 = atribuiReg();
				insere_inst("ADD", op1, op2, op3, quadrupla);	
			}
			else if(t->token == MINUS){
				op3 = atribuiReg();
				insere_inst("SUB", op1, op2, op3, quadrupla);		
			}
			else if(t->token == TIMES){
				op3 = atribuiReg();
				insere_inst("MULT", op1, op2, op3, quadrupla);		
			}
			else if(t->token == SLASH){
				op3 = atribuiReg();
				insere_inst("DIV", op1, op2, op3, quadrupla);		
			}
			else if(t->token == LT){
				op3 = atribuiReg();
				insere_inst("SLT", op1, op2, op3, quadrupla);	
			}
			else if(t->token == GT){
				op3 = atribuiReg();
				insere_inst("SGT", op1, op2, op3, quadrupla);	
			}
			else if(t->token == LE){
				op3 = atribuiReg();
				insere_inst("SLE", op1, op2, op3, quadrupla);	
			}
			else if(t->token == GE){
				op3 = atribuiReg();
				insere_inst("SGE", op1, op2, op3, quadrupla);	
			}
			else if(t->token == DIFFERENT){
				op3 = atribuiReg();
				insere_inst("SDT", op1, op2, op3, quadrupla);	
			}
			return op3;
		}
		else if(t->tipo == NumNode){
			return(t->lexema);
		}
		else if(t->tipo == VarNode){
			op1 = atribuiReg();
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
			op3 = atribuiReg();
			insere_inst("CALL", t->lexema, op2, op3, quadrupla);
			return(op3);
		}
		else if(t->tipo == Decl_varNode){
			insere_inst("ALLOC", t->lexema, func_atual, "-", quadrupla);
			if(strcmp(func_atual, "global") == 0){
				generateInterCode(t->sibling, quadrupla);
			}
		}
		else if(t->tipo == Decl_vetorNode){
			op3 = generateInterCode(t->childL, quadrupla);
			insere_inst("ALLOC", t->lexema, func_atual, op3, quadrupla);
			if(strcmp(func_atual, "global") == 0){
				generateInterCode(t->sibling, quadrupla);
			}
		}
		else if(t->tipo == VetorNode){
			op1 = atribuiReg();
			op2 = generateInterCode(t->childL, quadrupla);
			op3 = atribuiReg();
			insere_inst("MULT", op2, "4", op3, quadrupla);
			insere_inst("LOAD", op1, t->lexema, op3, quadrupla);
			return op1;
		}
		else if(t->tipo == WhileNode){
			TreeNode * aux;
			op2 = criaLabel();
			op3 = criaLabel();
			insere_inst("LAB", op2, "-", "-", quadrupla);
			op1 = generateInterCode(t->childL, quadrupla);
			insere_inst("IFF", op1, op3, "-", quadrupla);
			aux = t->childM;
			while(aux != NULL){
				generateInterCode(aux, quadrupla);
				aux = aux->sibling;
			}
			insere_inst("GOTO", op2, "-", "-", quadrupla);
			insere_inst("LAB", op3, "-", "-", quadrupla);
		}
		else{
			return NULL;
		}
	}
}


char * atribuiReg(char * var, char * escopo){
	char * temp;
	temp = (char *)malloc(sizeof(char)*10);
	if(registradores == NULL){
		registradores = criaRegs();
		insere_reg(var, escopo, cont_reg);
		sprintf(temp, "R%d", cont_reg);
		cont_reg++;
		return(temp);
	}
	else{
		if(registradores->inicio == NULL){
			insere_reg(var, escopo, cont_reg);
			sprintf(temp, "R%d", cont_reg);
			cont_reg++;
			return(temp);
		}
		else{
			Tcelula * aux;
			aux = registradores->inicio;
			while(aux->proximo != NULL){
				int t0 = strcmp(var, aux->var);
				int t1 = strcmp(escopo, aux->escopo);
				if(t0 == 0 && t1 == 0){
					sprintf(temp, "R%d", aux->reg);
					return(temp);
				}
			}
			insere_reg(var, escopo, cont_reg);
			sprintf(temp, "R%d", cont_reg);
			cont_reg++;
			return(temp);
		}
	}
}

char * criaLabel(){
	char * temp;
	temp = (char *) malloc(10*(sizeof(char)));
	sprintf(temp, "L%d", cont_label);
	cont_label++;
	return temp;
}

// analise semantica vetor arrumar