#include "globals.h"
#include "util.h"
#include "mem.h"
#include "assembly.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

FILE * saidaAssembly;
int assemblyLine = 0;
char * escopoAssembly;

int geraAssembly(Tquadruplas * quadruplas, TAssemblyCode * assemblyList){
    Tinst * aux = quadruplas->inicio;
    int param_count = 0;
    escopoAssembly = copiaString("global");
    insereEscopo("global");
    while(aux != NULL){
        if(strcmp("FUN", aux->instrucao) == 0){
            param_count = 0;
            insereAssemblyInst(aux->op2, "", "", "", NULL, assemblyList);
            insereEscopo(aux->op2);
            escopoAssembly = copiaString(aux->op2);
            if((strcmp(aux->proximo->instrucao, "ARG") != 0)&& (strcmp(aux->op2, "main") != 0)){
                insereAssemblyInst("addi", "sp", "sp", NULL, "3", assemblyList);
                insereVar(escopoAssembly, NULL, 1, JumpAddress, 0);
                insereVar(escopoAssembly, NULL, 1, SP, 0);
                insereVar(escopoAssembly, NULL, 1, FP, 0);
                insereAssemblyInst("str", "R63", "fp", NULL, getJumpAddress(escopoAssembly), assemblyList);
            }
            aux = aux->proximo;
        }
        else if(strcmp("GOTO", aux->instrucao) == 0){
            insereAssemblyInst("jmp", NULL, NULL, NULL, aux->op1, assemblyList);
            aux = aux->proximo;
        }
        else if(strcmp("LAB", aux->instrucao) == 0){
            insereAssemblyInst(aux->op1, NULL, NULL, NULL, NULL, assemblyList);
            aux = aux->proximo;
        }
        else if(strcmp("ASSIGN", aux->instrucao) == 0){//add ldrb in case of op2 is a imm
            if(aux->op2[0] == 'R'){
                insereAssemblyInst("mov", aux->op1, NULL, aux->op2, NULL, assemblyList);
                aux = aux->proximo;
            }
            else{
                insereAssemblyInst("ldrb", aux->op1, NULL, NULL, aux->op2, assemblyList);
                aux = aux->proximo;
            }
        }
        else if(strcmp("OUT", aux->instrucao) == 0){
            insereAssemblyInst("out", aux->op1, NULL, NULL, NULL, assemblyList);
            aux = aux->proximo;
        }
        else if(strcmp("IN", aux->instrucao) == 0){
            insereAssemblyInst("in", aux->op1, NULL, NULL, NULL, assemblyList);
            insereAssemblyInst("posin", NULL, NULL, NULL, NULL, assemblyList);
            aux = aux->proximo;
        }
        else if(strcmp("ADD", aux->instrucao) == 0){
            
            if(aux->op2[0] == 'R'){
                insereAssemblyInst("add", aux->res, aux->op1, aux->op2, NULL, assemblyList);
            }
            else{
                insereAssemblyInst("addi", aux->res, aux->op1, NULL, aux->op2, assemblyList);
            }
            aux = aux->proximo;
        }
        else if(strcmp("SUB", aux->instrucao) == 0){
            
            if(aux->op2[0] == 'R'){
                insereAssemblyInst("sub", aux->res, aux->op1, aux->op2, NULL, assemblyList);
            }
            else{
                insereAssemblyInst("subi", aux->res, aux->op1, NULL, aux->op2, assemblyList);
            }
            aux = aux->proximo;
        }
        else if(strcmp("MULT", aux->instrucao) == 0){
            
            if(aux->op2[0] == 'R'){
                insereAssemblyInst("mult", aux->res, aux->op1, aux->op2, NULL, assemblyList);
            }
            else{
                insereAssemblyInst("multi", aux->res, aux->op1, NULL, aux->op2, assemblyList);
            }
            aux = aux->proximo;
        }
        else if(strcmp("DIV", aux->instrucao) == 0){
            
            if(aux->op2[0] == 'R'){
                insereAssemblyInst("div", aux->res, aux->op1, aux->op2, NULL, assemblyList);
            }
            else{
                insereAssemblyInst("divi", aux->res, aux->op1, NULL, aux->op2, assemblyList);
            }
            aux = aux->proximo;
        }
        else if(strcmp("EQUAL", aux->instrucao) == 0){
            if(strcmp("IFF", aux->proximo->instrucao) == 0){
                if(aux->op2[0] == 'R'){
                    insereAssemblyInst("cmp", NULL, aux->op1, aux->op2, NULL, assemblyList);
                    insereAssemblyInst("bnq", NULL, NULL, NULL, aux->proximo->op2, assemblyList);
                }
                else{
                    insereAssemblyInst("cmpi", NULL, aux->op1, NULL, aux->op2, assemblyList);
                    insereAssemblyInst("bnq", NULL, NULL, NULL, aux->proximo->op2, assemblyList);
                }
            }
            aux = aux->proximo->proximo;
        }
        else if(strcmp("SLT", aux->instrucao) == 0){
            if(strcmp("IFF", aux->proximo->instrucao) == 0){
                if(aux->op2[0] == 'R'){
                    insereAssemblyInst("cmp", NULL, aux->op1, aux->op2, NULL, assemblyList);
                    insereAssemblyInst("beq", NULL, NULL, NULL, aux->proximo->op2, assemblyList);
                    insereAssemblyInst("cmp", NULL, aux->op1, aux->op2, NULL, assemblyList);
                    insereAssemblyInst("bgt", NULL, NULL, NULL, aux->proximo->op2, assemblyList);
                }
                else{
                    insereAssemblyInst("cmpi", NULL, aux->op1, NULL, aux->op2, assemblyList);
                    insereAssemblyInst("beq", NULL, NULL, NULL, aux->proximo->op2, assemblyList);
                    insereAssemblyInst("cmpi", NULL, aux->op1, NULL, aux->op2, assemblyList);
                    insereAssemblyInst("bgt", NULL, NULL, NULL, aux->proximo->op2, assemblyList);
                }
            }
            aux = aux->proximo->proximo;
        }
        else if(strcmp("SGT", aux->instrucao) == 0){
            if(strcmp("IFF", aux->proximo->instrucao) == 0){
                if(aux->op2[0] == 'R'){
                    insereAssemblyInst("cmp", NULL, aux->op1, aux->op2, NULL, assemblyList);
                    insereAssemblyInst("beq", NULL, NULL, NULL, aux->proximo->op2, assemblyList);
                    insereAssemblyInst("cmp", NULL, aux->op1, aux->op2, NULL, assemblyList);
                    insereAssemblyInst("blt", NULL, NULL, NULL, aux->proximo->op2, assemblyList);
                }
                else{
                    insereAssemblyInst("cmpi", NULL, aux->op1, NULL, aux->op2, assemblyList);
                    insereAssemblyInst("beq", NULL, NULL, NULL, aux->proximo->op2, assemblyList);
                    insereAssemblyInst("cmpi", NULL, aux->op1, NULL, aux->op2, assemblyList);
                    insereAssemblyInst("blt", NULL, NULL, NULL, aux->proximo->op2, assemblyList);
                }
            }
            aux = aux->proximo->proximo;
        }
        else if(strcmp("SGE", aux->instrucao) == 0){
            if(strcmp("IFF", aux->proximo->instrucao) == 0){
                if(aux->op2[0] == 'R'){
                    insereAssemblyInst("cmp", NULL, aux->op1, aux->op2, NULL, assemblyList);
                    insereAssemblyInst("blt", NULL, NULL, NULL, aux->proximo->op2, assemblyList);
                }
                else{
                    insereAssemblyInst("cmpi", NULL, aux->op1, NULL, aux->op2, assemblyList);
                    insereAssemblyInst("blt", NULL, NULL, NULL, aux->proximo->op2, assemblyList);
                }
            }
            aux = aux->proximo->proximo;
        }
        else if(strcmp("SLE", aux->instrucao) == 0){
            if(strcmp("IFF", aux->proximo->instrucao) == 0){
                if(aux->op2[0] == 'R'){
                    insereAssemblyInst("cmp", NULL, aux->op1, aux->op2, NULL, assemblyList);
                    insereAssemblyInst("bgt", NULL, NULL, NULL, aux->proximo->op2, assemblyList);
                }
                else{
                    insereAssemblyInst("cmpi", NULL, aux->op1, NULL, aux->op2, assemblyList);
                    insereAssemblyInst("bgt", NULL, NULL, NULL, aux->proximo->op2, assemblyList);
                }
            }
            aux = aux->proximo->proximo;
        }
        else if(strcmp("SDT", aux->instrucao) == 0){
            if(strcmp("IFF", aux->proximo->instrucao) == 0){
                if(aux->op2[0] == 'R'){
                    insereAssemblyInst("cmp", NULL, aux->op1, aux->op2, NULL, assemblyList);
                    insereAssemblyInst("beq", NULL, NULL, NULL, aux->proximo->op2, assemblyList);
                }
                else{
                    insereAssemblyInst("cmpi", NULL, aux->op1, NULL, aux->op2, assemblyList);
                    insereAssemblyInst("beq", NULL, NULL, NULL, aux->proximo->op2, assemblyList);
                }
            }
            aux = aux->proximo->proximo;
        }
        else if(strcmp("ARG", aux->instrucao) == 0){
            insereVar(escopoAssembly, aux->op2, 1, Args, aux->arrayType);
            insereAssemblyInst("addi", "sp", "sp", NULL, "1", assemblyList);
            if(strcmp(aux->proximo->instrucao, "ARG") != 0){
                insereAssemblyInst("addi", "sp", "sp", NULL, "3", assemblyList);
                insereVar(escopoAssembly, NULL, 1, SP, 0);
                insereVar(escopoAssembly, NULL, 1, FP, 0);
                insereVar(escopoAssembly, NULL, 1, JumpAddress, 0);
                insereAssemblyInst("str", "R63", "fp", NULL, getJumpAddress(escopoAssembly), assemblyList);
            }
            aux = aux->proximo;
        }
        else if(strcmp("END", aux->instrucao) == 0){
            if(strcmp("main", aux->op1) != 0){
                insereAssemblyInst("ldr", "sp", "fp", NULL, getSP(escopoAssembly), assemblyList);
                insereAssemblyInst("ldr", "R59", "fp", NULL, getJumpAddress(escopoAssembly), assemblyList);
                insereAssemblyInst("ldr", "fp", "fp", NULL, getFP(escopoAssembly), assemblyList);
                insereAssemblyInst("jmpr", "R59", NULL, NULL, NULL, assemblyList);
            }
            escopoAssembly = copiaString("global");
            aux = aux->proximo;
        }
        else if(strcmp("ALLOC", aux->instrucao) == 0){
            int num;
            int type;
            char * tamanho;
            if(strcmp(aux->res, "-") == 0){
                num = 1;
                tamanho = "1";
            }
            else{
                num = atoi(aux->res);
                tamanho = aux->res;
            }
            if(strcmp(escopoAssembly, "global") == 0){
                type = Heap;
            }
            else{
                type = Stack;
                insereAssemblyInst("addi", "sp", "sp", NULL, tamanho, assemblyList);
            }
            insereVar(escopoAssembly, aux->op1, num, type, aux->arrayType);
            aux = aux->proximo;
        }
        else if(strcmp("LOAD", aux->instrucao) == 0){
            TData * temp = is_global(aux->op2);
            int num;
            if(temp == NULL){//local
                temp = getData(escopoAssembly, aux->op2);
                if(temp->memoryPos == 0){
                    num = 1;
                }
                else{
                    num = temp->memoryPos;
                }
                char * posicao = (char *) malloc(sizeof(char)*(int)log10(num));
                sprintf(posicao, "%d", temp->memoryPos);
                if(temp->dataType == Args){
                    if(temp->array){
                        if(aux->res[0] == 'R'){
                            insereAssemblyInst("ldr", aux->op1, "fp", NULL, posicao, assemblyList);//load na pos da memoria que ele ta
                            insereAssemblyInst("add", aux->res, aux->res, aux->op1, NULL, assemblyList);//soma com pos_vet
                            insereAssemblyInst("ldr", aux->op1, aux->res, NULL, "0", assemblyList);//load no op1 o valor do vet[pos_vet]
                        }
                        else if(aux->res[0] == '-'){
                            insereAssemblyInst("ldr", aux->op1, "fp", NULL, posicao, assemblyList);
                        }
                        else{
                            insereAssemblyInst("ldr", "R58", "fp", NULL, posicao, assemblyList);
                            insereAssemblyInst("addi", "R58", "R58", NULL, aux->res, assemblyList);
                            insereAssemblyInst("ldr", aux->op1, "R58", NULL, "0", assemblyList);      
                        }
                    }
                    else{
                        insereAssemblyInst("ldr", aux->op1, "fp", NULL, posicao, assemblyList);
                    }
                }
                else{//stack
                    if(temp->array == 1){
                        if(aux->res[0] == 'R'){
                            //insereAssemblyInst("addi", aux->res, aux->res, NULL, posicao, assemblyList);
                            insereAssemblyInst("add", aux->res, aux->res, "fp", NULL, assemblyList);
                            insereAssemblyInst("ldr", aux->op1, aux->res, NULL, posicao, assemblyList);
                        }
                        else if(aux->res[0] == '-'){
                            insereAssemblyInst("addi", aux->op1, "fp", NULL, posicao, assemblyList);
                        }
                        else{
                            insereAssemblyInst("addi", "R58", "fp", NULL, aux->res, assemblyList);
                            insereAssemblyInst("ldr", aux->op1, "R58", NULL, posicao, assemblyList);
                        }
                    }
                    else{
                        insereAssemblyInst("ldr", aux->op1, "fp", NULL, posicao, assemblyList);
                    }
                }
            }
            else{
                if(temp->memoryPos == 0){
                    num = 1;
                }
                else{
                    num = temp->memoryPos;
                }
                char * posicao = (char *) malloc(sizeof(char)*(int)log10(num));
                sprintf(posicao, "%d", temp->memoryPos);
                if(temp->array){
                    if(aux->res[0] == 'R'){
                        insereAssemblyInst("ldr", aux->op1, aux->res, NULL, posicao, assemblyList);
                    }
                    else if(aux->res[0] == '-'){
                        insereAssemblyInst("ldrb", aux->op1, NULL, NULL, posicao, assemblyList);
                    }
                    else{
                        insereAssemblyInst("ldrb", "R58", NULL, NULL, posicao, assemblyList);
                        insereAssemblyInst("ldr", aux->op1, "R58", NULL, aux->res, assemblyList);
                    }
                }
                else{
                    insereAssemblyInst("ldrb", "R58", NULL, NULL, "0", assemblyList);
                    insereAssemblyInst("ldr", aux->op1, "R58", NULL, posicao, assemblyList);
                }
            }
            aux = aux->proximo;
        }
        else if(strcmp("STORE", aux->instrucao) == 0){
            TData * temp = is_global(aux->op2);
            if(temp != NULL){//global
                int num;
                if(temp->memoryPos == 0){
                    num = 1;
                }
                else{
                    num = temp->memoryPos;
                }
                char * posicao = (char *) malloc(sizeof(char)*(int)log10(num));
                sprintf(posicao, "%d", temp->memoryPos);
                if(temp->array){
                    if(aux->res[0] == 'R'){
                        insereAssemblyInst("str", aux->op1, aux->res, NULL, posicao, assemblyList);
                    }
                    else{
                        insereAssemblyInst("ldrb", "R58", NULL, NULL, posicao, assemblyList);
                        insereAssemblyInst("str", aux->op1, "R58", NULL, aux->res, assemblyList);
                    }
                }
                else{
                    insereAssemblyInst("ldrb", "R58", NULL, NULL, posicao, assemblyList);
                    insereAssemblyInst("str", aux->op1, "R58", NULL, "0", assemblyList);
                }
            }
            else{//local
                temp = getData(escopoAssembly, aux->op2);
                int num;
                if(temp->memoryPos == 0){
                    num = 1;
                }
                else{
                    num = temp->memoryPos;
                }
                char * posicao = (char *) malloc(sizeof(char)*(int)log10(num));
                sprintf(posicao, "%d", temp->memoryPos);
                if(temp->array){
                    if(temp->dataType == Args){//(STORE, R9, a, R4)
                        if(aux->res[0] == 'R'){
                            insereAssemblyInst("ldr", "R58", "fp", NULL, posicao, assemblyList); 
                            insereAssemblyInst("add", aux->res, aux->res, "R58", NULL, assemblyList);
                            insereAssemblyInst("str", aux->op1, aux->res, NULL, "0", assemblyList);
                        }
                        else{
                            insereAssemblyInst("ldr", "R58", "fp", NULL, posicao, assemblyList); 
                            insereAssemblyInst("addi", "R58", "R58", NULL, aux->res, assemblyList);
                            insereAssemblyInst("str", aux->op1, aux->res, NULL, "0", assemblyList);
                        }
                    }
                    else{//stack
                        if(aux->res[0] == 'R'){
                            insereAssemblyInst("mov", "R58", NULL, "fp", NULL, assemblyList); 
                            insereAssemblyInst("add", "R58", "R58", aux->res, NULL, assemblyList);
                            insereAssemblyInst("str", aux->op1, "R58", NULL, posicao, assemblyList);
                        }
                        else{
                            insereAssemblyInst("mov", "R58", NULL, "fp", NULL, assemblyList); 
                            insereAssemblyInst("addi", "R58", "R58", NULL, aux->res, assemblyList);
                            insereAssemblyInst("str", aux->op1, "R58", NULL, posicao, assemblyList);
                        }
                    }
                }
                else{
                    insereAssemblyInst("str", aux->op1, "fp", NULL, posicao, assemblyList);
                }
            }
            aux = aux->proximo;
        }
        else if(strcmp("PARAM", aux->instrucao) == 0){
            param_count++;
            char * posicao = (char *) malloc(sizeof(char)*(int)log10(param_count));
            sprintf(posicao, "%d", param_count);
            if(aux->op1[0] == 'R'){
                insereAssemblyInst("str", aux->op1, "sp", NULL, posicao, assemblyList);
            }
            else{
                insereAssemblyInst("ldrb", "R58", NULL, NULL, aux->op1, assemblyList);
                insereAssemblyInst("str", "R58", "sp", NULL, posicao, assemblyList);
            }
            aux = aux->proximo;
        }
        else if(strcmp("CALL", aux->instrucao) == 0){
            param_count++;
            char * posicaoSP = (char *) malloc(sizeof(char)*(int)log10(param_count));
            sprintf(posicaoSP, "%d", param_count);
            insereAssemblyInst("str", "sp", "sp", NULL, posicaoSP, assemblyList);
            param_count++;
            char * posicaoFP = (char *) malloc(sizeof(char)*(int)log10(param_count));
            sprintf(posicaoFP, "%d", param_count);
            insereAssemblyInst("str", "fp", "sp", NULL, posicaoFP, assemblyList);
            insereAssemblyInst("addi", "sp", "sp", NULL, "1", assemblyList);
            insereAssemblyInst("mov", "fp", NULL, "sp", NULL, assemblyList);
            insereAssemblyInst("jal", NULL, NULL, NULL, aux->op1, assemblyList);
            insereAssemblyInst("mov", aux->res, NULL, "R60", NULL, assemblyList);
            param_count = 0;
            aux = aux->proximo;
        }
        else if(strcmp("RET", aux->instrucao) == 0){
            if(aux->op1[0] != '-'){
                if(aux->op1[0] == 'R'){
                    insereAssemblyInst("mov", "R60", NULL, aux->op1, NULL, assemblyList);
                }
                else{
                    insereAssemblyInst("ldrb", "R60", NULL, NULL, aux->op1, assemblyList);
                }
            }
            aux = aux->proximo;
        }
        else if(strcmp("HALT", aux->instrucao) == 0){
            insereAssemblyInst("halt", NULL, NULL, NULL, NULL, assemblyList);
            aux = aux->proximo;
        }
        else{
            aux = aux->proximo;
        }
    }
    TMemory * escopoGlobal = getEscopo("global");
    char * fp_inicial = (char *) malloc(sizeof(char)*(int)log10(escopoGlobal->tamEscopo + 1));
    sprintf(fp_inicial, "%d", escopoGlobal->tamEscopo);
    insereInicioAssemblyInst("jmp", NULL, NULL, NULL, "main", assemblyList);
    insereInicioAssemblyInst("ldrb", "sp", NULL, NULL, "0", assemblyList);
    insereInicioAssemblyInst("ldrb", "fp", NULL, NULL, fp_inicial, assemblyList);
    return 0;
}

TAssemblyCode * criaAssemblyCode(){
    TAssemblyCode * assemblyList;
    assemblyList = (TAssemblyCode *)malloc(sizeof(TAssemblyCode));
    assemblyList->inicio = NULL;
    assemblyList->tam = 0;
    return assemblyList;
}

void imprimeAssembly(TAssemblyCode * assemblyList){
    TAssemblyInst * aux;
    int line = 0;
    aux = assemblyList->inicio;
    while(aux != NULL){
        fprintf(saidaAssembly, "%d: ", line);
        fprintf(saidaAssembly, "%s ", aux->instrucao);
        if(aux->rd != NULL){
            fprintf(saidaAssembly, "%s ", aux->rd);
        }
        if(aux->rn != NULL){
            fprintf(saidaAssembly, "%s ", aux->rn);
        }
        if(aux->rm != NULL){
            fprintf(saidaAssembly, "%s ", aux->rm);
        }
        if(aux->imm != NULL){
            fprintf(saidaAssembly, "%s ", aux->imm);
        }
        fprintf(saidaAssembly, "\n");
        aux = aux->proximo;
        line++;
    }
    return;
}

void insereAssemblyInst(char * instrucao, char * rd, char * rn, char * rm, char * imm, TAssemblyCode * assemblyList){
	TAssemblyInst * inst, * aux;
	inst = (TAssemblyInst *) malloc(sizeof(TAssemblyInst));
	inst->instrucao = copiaString(instrucao);
	inst->rd = copiaString(rd);
	inst->rm = copiaString(rm);
	inst->rn = copiaString(rn);
    inst->imm = copiaString(imm);
	inst->line = assemblyLine;
	inst->proximo = NULL;
	assemblyLine++;
	if(assemblyList->inicio == NULL){
		assemblyList->inicio = inst;
		assemblyList->tam = assemblyList->tam + 1;
	}
	else{
		aux = assemblyList->inicio;
		while(aux->proximo != NULL){
			aux = aux->proximo;
		}
		aux->proximo = inst;
		assemblyList->tam = assemblyList->tam + 1;
	}
    return;
}

void insereInicioAssemblyInst(char * instrucao, char * rd, char * rm, char * rn, char * imm, TAssemblyCode * assemblyList){
	TAssemblyInst * inst;
	inst = (TAssemblyInst *) malloc(sizeof(TAssemblyInst));
	inst->instrucao = copiaString(instrucao);
	inst->rd = copiaString(rd);
	inst->rm = copiaString(rm);
	inst->rn = copiaString(rn);
    inst->imm = copiaString(imm);
	inst->line = assemblyLine;
	inst->proximo = assemblyList->inicio;
	assemblyLine++;
	assemblyList->inicio = inst;
	assemblyList->tam = assemblyList->tam + 1;
    return;
}