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
			if(buscar(t->lexema, escopo, t->tipo, tabela[n]->inicio) == NULL){
				insere(t->lineno, t->lexema, escopo, t->token, t->tipo, tabela[n]);
			}
			else{
				semantic_error(t->lineno, t->lexema);
			}
			escopo = copiaString(t->lexema);
		}
		else if ((t->tipo == VarNode) || (t->tipo == VetorNode)){
			n = hash(t->lexema, "global");
			if(buscar(t->lexema, "global", t->tipo, tabela[n]->inicio) == NULL){
				n = hash(t->lexema, escopo);
				if(buscar(t->lexema, escopo, t->tipo, tabela[n]->inicio) == NULL){
					semantic_error(t->lineno, t->lexema);
				}
				else{
					insere(t->lineno, t->lexema, escopo, t->token, t->tipo, tabela[n]);
				}
			}
			else{
				insere(t->lineno, t->lexema, escopo, t->token, t->tipo, tabela[n]);
			}
		}
		else if((t->tipo == Decl_varNode) || (t->tipo == Decl_vetorNode) || (t->tipo == VetorParamNode) || (t->tipo == ParamNode)){
			if(t->token == VOID){
				semantic_error(t->lineno, t->lexema);
			}
			n = hash(t->lexema, "global");
			if(buscar(t->lexema, "global", t->tipo, tabela[n]->inicio) == NULL){
				n = hash(t->lexema, escopo);
				if(buscar(t->lexema, escopo, t->tipo, tabela[n]->inicio) == NULL){
					insere(t->lineno, t->lexema, escopo, t->token, t->tipo, tabela[n]);		
				}
				else{
					semantic_error(t->lineno, t->lexema);
				}
			}
			else{
				semantic_error(t->lineno, t->lexema);
			}
		}
		else if(t->tipo == OpNode){
			if(verificaTipo(t) == -1){
				semantic_error(t->lineno, t->lexema);
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

void semantic_error(int linha, char * lexema){
	if(linha == 0){
		printf("semantic error: missing main function\n");
	}
	else{
		printf("semantic error at line %d, current token: %s.\n", linha, lexema);
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
		aux = buscar(t->lexema, "global", t->tipo, tabela[n]->inicio);
		if(aux == NULL){
			return -1;
		}
		else{
			return aux->tipo_id;
		}
	}
}

//decl global uso local = tudo certo
//decl local uso no msm local = tudo certo
//decl local uso em outro local = var do outro local n declarada
//decl local uso global = var n declarada