#include "globals.h"
#include "util.h"
#include "symtab.h"
#include "parser.tab.h"

Tlista * tabela[211];
char * escopo;
FILE * saidaTabela;

Tlista * criaLista()
{
    Tlista * aux;
    aux = (Tlista *) malloc(sizeof(Tlista));
    aux->inicio = NULL;
    aux->tam = 0;
    return aux;
}

void insere(int linhas, char * lexema, char * escopo, int tipo_id, int tipo_node, Tlista * linha)
{
    Tno * no;
    no = (Tno*) malloc(sizeof(Tno));
    no->linhas = linhas;
    no->lexema = copiaString(lexema);
    no->escopo = copiaString(escopo);
    no->tipo_id = tipo_id;
    no->tipo_node = tipo_node;
    no->proximo = NULL;
    if (linha->inicio == NULL)
    {
        linha->inicio = no;
    }
    else
    {
        Tno * aux;
        Tno * anterior = NULL;
        aux = linha->inicio;
        while (aux != NULL)
        {
            anterior = aux;
            aux = aux->proximo;
        }
       anterior->proximo = no;
    }
}

void deleta(Tno * no, Tlista * linha)
{
    if (no == linha->inicio)
    {
        linha->inicio = no->proximo;
        linha->tam--;
        free(no);
    }
    else
    {
        Tno * aux;
        Tno * anterior;
        aux = linha->inicio;
        while (aux != no)
        {
            anterior = aux;
            aux = aux->proximo;
        }
        anterior->proximo = no->proximo;
        linha->tam--;
        free(no);
    }
}

Tno * buscar(char * lexema, char * escopo, int tipo_node, Tno * inicio)
{
    while (inicio != NULL)
    {
    	int t0 = strcmp(inicio->lexema, lexema);
    	int t1 = strcmp(inicio->escopo, escopo);
        if ((t0 == 0)&&(t1 == 0) /*&& (verificaTipoNode(inicio->tipo_node, tipo_node) == 0)*/)
        {
            return inicio;
        }
        else
        {
            inicio = inicio->proximo;
        }
    }
    return NULL;
}

void imprime(Tlista * linha)
{
    if (linha->inicio != NULL)
    {
        Tno * aux = NULL;
        aux = linha->inicio;
        while (aux != NULL)
        {
            fprintf(saidaTabela, " lexema: %s, escopo: %s ", aux->lexema, aux->escopo);
            if(aux->tipo_id == VOID){
            	fprintf(saidaTabela, "tipo: VOID, linha: %d -> ", aux->linhas);
            }
            else if(aux->tipo_id == INT){
            	fprintf(saidaTabela, "tipo: INT, linha: %d -> ", aux->linhas);
            }
            aux = aux->proximo;
        }
    }
}

int hash(char * nome, char * escopo)
{
	int temp = 0;
	int i = 0;
	while(nome[i] != '\0'){
		temp = ((temp << 4) + nome[i]) % 211;
		i = i + 1;
	}
	i = 0;
	while(escopo[i] != '\0'){
		temp = ((temp << 4) + escopo[i]) % 211;
		i = i + 1;
	}
    return(temp);
}

int verificaTipoNode(int t1, int t2){
    if(t1 == t2){
        return 0;
    }
    else if((t1 == Decl_varNode && t2 == VarNode)||(t1 == ParamNode && t2 == VarNode)){
        return 0;
    }
    else if((t1 == Decl_vetorNode && t2 == VetorNode)||(t1 == VetorParamNode && t2 == VetorNode)){
        return 0;
    }
    else if(t1 == FunNode && t2 == AtivNode){
        return 0;
    }
    else{
        return -1;
    }
}