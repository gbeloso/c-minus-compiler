#include "globals.h"
#include "util.h"
#include "mem.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

TMemoryList * memoriaLista;
FILE * saidaMem;

TMemoryList * criaListaMemoria(){
    TMemoryList * aux = (TMemoryList *)malloc(sizeof(TMemoryList));
    aux->inicio = NULL;
    return(aux);
}

TMemory * criaMemoria(char * escopo){
    TMemory * aux = (TMemory *)malloc(sizeof(TMemory));
    aux->escopo = escopo;
    aux->tamEscopo = 0;
    aux->dataList = NULL;
    return aux;
}

TData * criaData(char * dataName, int size, int memoryPos, int varType, int array){
    TData * aux = (TData *)malloc(sizeof(TData));
    aux->dataName = dataName;
    aux->size = size;
    aux->memoryPos = memoryPos;
    aux->dataType = varType;
    aux->proximo = NULL;
    aux->array = array;
    return aux;
}

void imprimeMemoriaLista(TMemoryList * lista){
    TMemory * aux;
    saidaMem = fopen("saida/mem.txt", "w");
    aux = lista->inicio;
    while(aux != NULL){
        fprintf(saidaMem, "%s, tam: %d\n", aux->escopo, aux->tamEscopo);
        imprimeMemoria(aux);
        aux = aux->proximo;
    }
}

void imprimeMemoria(TMemory * memoria){
    imprimeData(memoria->dataList);
}

void imprimeData (TData * data){
    TData * aux;
    aux = data;
    while(aux != NULL){
        fprintf(saidaMem, "\t%d: name: %s, size: %d type: ", aux->memoryPos, aux->dataName, aux->size);
        if(aux->dataType == Heap){
            fprintf(saidaMem, "Heap");
        }
        else if(aux->dataType == Stack){
            fprintf(saidaMem, "Stack");
        }
        else if(aux->dataType == Args){
            fprintf(saidaMem, "Arg");
        }
        else if(aux->dataType == Params){
            fprintf(saidaMem, "Param");
        }
        else if(aux->dataType == JumpAddress){
            fprintf(saidaMem, "Link");
        }
        else if(aux->dataType == SP){
            fprintf(saidaMem, "SP");
        }
        else if(aux->dataType == FP){
            fprintf(saidaMem, "FP");
        }
        fprintf(saidaMem, " vetor: %d\n", aux->array);
        aux = aux->proximo;
    }
}


TData * getData(char * escopo, char * dataName){
    TData * globalPos = is_global(dataName);
    if(globalPos != NULL){
        return globalPos;
    }
    else{
        TMemory * func_aux = memoriaLista->inicio;
        while((func_aux != NULL) && (strcmp(func_aux->escopo, escopo) != 0)){
            func_aux = func_aux->proximo;
        }
        TData * aux = func_aux->dataList;
        while(aux != NULL){
            if(aux->dataName != NULL){
                if(strcmp(aux->dataName, dataName) == 0){
                    return aux;
                }
            }
            aux = aux->proximo;
        }
    }
    return NULL;
}

TData * is_global(char * var){
    TMemory * func_aux = memoriaLista->inicio;
    while((func_aux != NULL)&&(strcmp("global", func_aux->escopo) != 0)){
        func_aux = func_aux->proximo;
    }
    if(func_aux == NULL){
        return NULL;
    }
    else{
        TData * aux = func_aux->dataList;
        while((aux != NULL)&&(strcmp(var, aux->dataName) != 0)){
            aux = aux->proximo;
        }
        if(aux == NULL){
            return NULL;
        }
        else{
            return aux;
        }
    }
    return NULL;
}

void insereEscopo(char * escopo){
    TMemory * aux = criaMemoria(escopo);
    if(memoriaLista->inicio == NULL){
        memoriaLista->inicio = aux;
        memoriaLista->fim = aux;
    }
    else{
        memoriaLista->fim->proximo = aux;
        memoriaLista->fim = aux;
    }
}

void insereVar(char * escopo, char * var, int size, int varType, int array){
    TMemory * aux = memoriaLista->inicio;
    while(aux != NULL){
        if(strcmp(aux->escopo, escopo) == 0){
            break;
        }
        aux = aux->proximo;
    }
    if(aux->dataList == NULL){
        aux->dataList = criaData(var, size, 0, varType, array);
        aux->tamEscopo = aux->tamEscopo + size;
    }
    else{
        TData * temp = aux->dataList;
        while(temp->proximo != NULL){
            temp = temp->proximo;
        }
        temp->proximo = criaData(var, size, temp->memoryPos + temp->size, varType, array);
        aux->tamEscopo = aux->tamEscopo + size;
    }
}

char * getJumpAddress(char * escopo){
    TMemory * aux = memoriaLista->inicio;
    char * retorno;
    int num;
    while(aux != NULL){
        if(strcmp(aux->escopo, escopo) == 0){
            break;
        }
        aux = aux->proximo;
    }
    TData * temp = aux->dataList;
    while(temp != NULL){
        if(temp->dataType == JumpAddress){
            if(temp->memoryPos == 0){
                num = 1;
            }
            else{
                num = temp->memoryPos;
            }
            retorno  = (char *) malloc(sizeof(char)*(int)log10(num));
            sprintf(retorno, "%d", temp->memoryPos);
            return retorno;
        }
        temp = temp->proximo;
    }
    return NULL;
}

char * getSP(char * escopo){
    TMemory * aux = memoriaLista->inicio;
    char * retorno;
    int num;
    while(aux != NULL){
        if(strcmp(aux->escopo, escopo) == 0){
            break;
        }
        aux = aux->proximo;
    }
    TData * temp = aux->dataList;
    while(temp != NULL){
        if(temp->dataType == SP){
            if(temp->memoryPos == 0){
                num = 1;
            }
            else{
                num = temp->memoryPos;
            }
            retorno  = (char *) malloc(sizeof(char)*(int)log10(num));
            sprintf(retorno, "%d", temp->memoryPos);
            return retorno;
        }
        temp = temp->proximo;
    }
    return NULL;
}

char * getFP(char * escopo){
    TMemory * aux = memoriaLista->inicio;
    char * retorno;
    int num;
    while(aux != NULL){
        if(strcmp(aux->escopo, escopo) == 0){
            break;
        }
        aux = aux->proximo;
    }
    TData * temp = aux->dataList;
    while(temp != NULL){
        if(temp->dataType == FP){
            if(temp->memoryPos == 0){
                num = 1;
            }
            else{
                num = temp->memoryPos;
            }
            retorno  = (char *) malloc(sizeof(char)*(int)log10(num));
            sprintf(retorno, "%d", temp->memoryPos);
            return retorno;
        }
        temp = temp->proximo;
    }
    return NULL;
}

TMemory * getEscopo(char * escopo){
    TMemory * aux = memoriaLista->inicio;
    while(aux != NULL){
        if(strcmp(escopo, aux->escopo) == 0){
            return aux;
        }
        aux = aux->proximo;
    }
    return NULL;
}