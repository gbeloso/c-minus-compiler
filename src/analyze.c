#include "globals.h"
#include "util.h"
#include "analyze.h"
#include "symtab.h"
#include "parser.tab.h"
#include <string.h>
#include <signal.h>
#include <unistd.h>

//Tlista * tabela[211];

void analyze(TreeNode * t){
	Tno * aux;
	int n;
	if(t==NULL){
		return;
	}	
	else{
		if(t->tipo == FunNode){
			n = hash(t->lexema, escopo);
			if(buscar(t->lexema, escopo, tabela[n]->inicio) == NULL){
				insere(t->lineno, t->lexema, escopo, t->token, tabela[n]);
			}
			else{
				semantic_error(t->lineno);
			}
			escopo = copiaString(t->lexema);
		}
		else if ((t->tipo == VarNode) || (t->tipo == VetorNode)){
			n = hash(t->lexema, "global");
			if(buscar(t->lexema, "global", tabela[n]->inicio) == NULL){
				n = hash(t->lexema, escopo);
				if(buscar(t->lexema, escopo, tabela[n]->inicio) == NULL){
					semantic_error(t->lineno);
				}
				else{
					insere(t->lineno, t->lexema, escopo, t->token, tabela[n]);
				}
			}
			else{
				insere(t->lineno, t->lexema, escopo, t->token, tabela[n]);
			}
		}
		else if((t->tipo == Decl_varNode) || (t->tipo == Decl_vetorNode) || (t->tipo == VetorParamNode) || (t->tipo == ParamNode)){
			if(t->token == VOID){
				semantic_error(t->lineno);
			}
			n = hash(t->lexema, "global");
			if(buscar(t->lexema, "global", tabela[n]->inicio) == NULL){
				n = hash(t->lexema, escopo);
				if(buscar(t->lexema, escopo, tabela[n]->inicio) == NULL){
					insere(t->lineno, t->lexema, escopo, t->token, tabela[n]);		
				}
				else{
					semantic_error(t->lineno);
				}
			}
			else{
				semantic_error(t->lineno);
			}
		}
		else if(t->tipo == OpNode){
			if(verificaTipo(t) == -1){
				semantic_error(t->lineno);
			}
		}
		analyze(t->childL);
		analyze(t->childM);
		analyze(t->childR);
		if(t->tipo == FunNode){
			escopo = copiaString("global");
		}
		analyze(t->sibling);
	}
		

}

void semantic_error(int linha){
	if(linha == 0){
		printf("semantic error: missing main function\n");
	}
	else{
		printf("semantic error at line %d.\n", linha);
	}
	int p_id = getpid();
	kill(p_id, SIGKILL);
}

int verificaTipo(TreeNode * t){
	int n, le, ld;
	Tno * aux;
	if(t->tipo == OpNode){
		le = verificaTipo(t->childL);
		ld = verificaTipo(t->childM);
		if(le == ld){
			return le;
		}
		else{
			return -1;
		}
	}
	else if(t->tipo == NumNode){
		return t->token;
	}
	else if((t->tipo == VarNode) || (t->tipo == VetorNode)){
		return t->token;
	}
	else if(t->tipo == AtivNode){
		n = hash(t->lexema, "global");
		aux = buscar(t->lexema, "global", tabela[n]->inicio);
		if(aux == NULL){
			return -1;
		}
		else{
			return aux->tipo;
		}
	}
}

//decl global uso local = tudo certo
//decl local uso no msm local = tudo certo
//decl local uso em outro local = var do outro local n declarada
//decl local uso global = var n declarada