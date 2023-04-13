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

void insere(int linhas, char * lexema, char * escopo, int tipo, Tlista * linha)
{
    Tno * no;
    no = (Tno*) malloc(sizeof(Tno));
    no->linhas = linhas;
    no->lexema = copiaString(lexema);
    no->escopo = copiaString(escopo);
    no->tipo = tipo;
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

Tno * buscar(char * lexema, char * escopo, Tno * inicio)
{
    while (inicio != NULL)
    {
    	int t0 = strcmp(inicio->lexema, lexema);
    	int t1 = strcmp(inicio->escopo, escopo);
        if ((t0 == 0)&&(t1 == 0))
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
            if(aux->tipo == VOID){
            	fprintf(saidaTabela, "tipo: VOID, linha: %d -> ", aux->linhas);
            }
            else if(aux->tipo == INT){
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

