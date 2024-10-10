#include "globals.h"
#include "util.h"
#include "binary.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

TBinCode * criaBinCode(){
    TBinCode * aux;
    aux = (TBinCode *)malloc(sizeof(TBinCode));
    aux->inicio = NULL;
    aux->fim = NULL;
    aux->tam = 0;
    return aux;
}

TBinInst * criaBinInst(char * instrucao){
    TBinInst * aux = (TBinInst *)malloc(sizeof(TBinInst));
    aux->instr = copiaString(instrucao);
    aux->proximo = NULL;
    return aux;
}

void insereBinInst(TBinInst * noInst, TBinCode * binCode){
    if(binCode->inicio == NULL){
        binCode->inicio = noInst;
        binCode->fim = noInst;
    }
    else{
        binCode->fim->proximo = noInst;
        binCode->fim = noInst;
    }
    noInst->proximo = NULL;
}

void imprimeBinCode(TBinCode * binCodeList){
    FILE * saidaBin;
    saidaBin = fopen("saida/bin.txt", "w");
    TBinInst * aux = binCodeList->inicio;
    int line = 0;
    while(aux != NULL){
        fprintf(saidaBin, "%s\n", aux->instr);
        line++;
        aux = aux->proximo;
    }
    return;
}

void geraBinCode(TBinCode * binCodeList, TAssemblyCode * assemblyCodeList){
    TAssemblyInst * aux = assemblyCodeList->inicio;
    while(aux != NULL){
        if(strcmp("add", aux->instrucao) == 0){
            char * rd = convertReg(aux->rd);
            char * rm = convertReg(aux->rm);
            char * rn = convertReg(aux->rn);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00000000");
            strcat(operacao, rn);
            strcat(operacao, rd);
            strcat(operacao, "000000");
            strcat(operacao, rm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("addi", aux->instrucao) == 0){
            char * rd = convertReg(aux->rd);
            int n = atoi(aux->imm);
            char * imm = toBinaryString(n, 8);
            char * rn = convertReg(aux->rn);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00000001");
            strcat(operacao, rn);
            strcat(operacao, rd);
            strcat(operacao, "0000");
            strcat(operacao, imm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("sub", aux->instrucao) == 0){
            char * rd = convertReg(aux->rd);
            char * rm = convertReg(aux->rm);
            char * rn = convertReg(aux->rn);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00000010");
            strcat(operacao, rn);
            strcat(operacao, rd);
            strcat(operacao, "000000");
            strcat(operacao, rm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("subi", aux->instrucao) == 0){
            char * rd = convertReg(aux->rd);
            int n = atoi(aux->imm);
            char * imm = toBinaryString(n, 8);
            char * rn = convertReg(aux->rn);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00000011");
            strcat(operacao, rn);
            strcat(operacao, rd);
            strcat(operacao, "0000");
            strcat(operacao, imm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("cmp", aux->instrucao) == 0){
            char * rm = convertReg(aux->rm);
            char * rn = convertReg(aux->rn);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00000100");
            strcat(operacao, rn);
            strcat(operacao, "000000000000");
            strcat(operacao, rm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("cmpi", aux->instrucao) == 0){
            int n = atoi(aux->imm);
            char * imm = toBinaryString(n, 8);
            char * rn = convertReg(aux->rn);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00000101");
            strcat(operacao, rn);   
            strcat(operacao, "0000000000");
            strcat(operacao, imm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("mov", aux->instrucao) == 0){
            char * rd = convertReg(aux->rd);
            char * rm = convertReg(aux->rm);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00000110");
            strcat(operacao, "000000");
            strcat(operacao, rd);
            strcat(operacao, "000000");
            strcat(operacao, rm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("mult", aux->instrucao) == 0){
            char * rd = convertReg(aux->rd);
            char * rm = convertReg(aux->rm);
            char * rn = convertReg(aux->rn);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00000111");
            strcat(operacao, rn);
            strcat(operacao, rd);
            strcat(operacao, "000000");
            strcat(operacao, rm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("multi", aux->instrucao) == 0){
            char * rd = convertReg(aux->rd);
            int n = atoi(aux->imm);
            char * imm = toBinaryString(n, 8);
            char * rn = convertReg(aux->rn);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00001000");
            strcat(operacao, rn);
            strcat(operacao, rd);
            strcat(operacao, "0000");
            strcat(operacao, imm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("div", aux->instrucao) == 0){
            char * rd = convertReg(aux->rd);
            char * rm = convertReg(aux->rm);
            char * rn = convertReg(aux->rn);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00001001");
            strcat(operacao, rn);
            strcat(operacao, rd);
            strcat(operacao, "000000");
            strcat(operacao, rm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("divi", aux->instrucao) == 0){
            char * rd = convertReg(aux->rd);
            int n = atoi(aux->imm);
            char * imm = toBinaryString(n, 8);
            char * rn = convertReg(aux->rn);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00001010");
            strcat(operacao, rn);
            strcat(operacao, rd);
            strcat(operacao, "0000");
            strcat(operacao, imm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("str", aux->instrucao) == 0){
            char * rd = convertReg(aux->rd);
            int n = atoi(aux->imm);
            char * imm = toBinaryString(n, 12);
            char * rn = convertReg(aux->rn);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00001011");
            strcat(operacao, rn);
            strcat(operacao, rd);
            strcat(operacao, imm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("ldr", aux->instrucao) == 0){
            char * rd = convertReg(aux->rd);
            int n = atoi(aux->imm);
            char * imm = toBinaryString(n, 12);
            char * rn = convertReg(aux->rn);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00001100");
            strcat(operacao, rn);
            strcat(operacao, rd);
            strcat(operacao, imm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("ldrb", aux->instrucao) == 0){
            char * rd = convertReg(aux->rd);
            int n = atoi(aux->imm);
            char * imm = toBinaryString(n, 12);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00001101");
            strcat(operacao, "000000");
            strcat(operacao, rd);
            strcat(operacao, imm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("beq", aux->instrucao) == 0){
            int n = getLabelLine(aux->imm, assemblyCodeList);
            char * imm = toBinaryString(n, 10);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00001110");
            strcat(operacao, "00000000000000");
            strcat(operacao, imm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("bnq", aux->instrucao) == 0){
            int n = getLabelLine(aux->imm, assemblyCodeList);
            char * imm = toBinaryString(n, 10);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00001111");
            strcat(operacao, "00000000000000");
            strcat(operacao, imm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("bgt", aux->instrucao) == 0){
            int n = getLabelLine(aux->imm, assemblyCodeList);
            char * imm = toBinaryString(n, 10);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00010000");
            strcat(operacao, "00000000000000");
            strcat(operacao, imm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("blt", aux->instrucao) == 0){
            int n = getLabelLine(aux->imm, assemblyCodeList);
            char * imm = toBinaryString(n, 10);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00010001");
            strcat(operacao, "00000000000000");
            strcat(operacao, imm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("jmp", aux->instrucao) == 0){
            int n = getLabelLine(aux->imm, assemblyCodeList);
            char * imm = toBinaryString(n, 10);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00010010");
            strcat(operacao, "00000000000000");
            strcat(operacao, imm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("jal", aux->instrucao) == 0){
            int n = getLabelLine(aux->imm, assemblyCodeList);
            char * imm = toBinaryString(n, 10);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00010011");
            strcat(operacao, "00000000000000");
            strcat(operacao, imm);
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("jmpr", aux->instrucao) == 0){
            char * rd = convertReg(aux->rd);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00010100");
            strcat(operacao, "000000");
            strcat(operacao, rd);
            strcat(operacao, "000000000000");
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("in", aux->instrucao) == 0){
            char * rd = convertReg(aux->rd);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00010101");
            strcat(operacao, "000000");
            strcat(operacao, rd);
            strcat(operacao, "000000000000");
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("posin", aux->instrucao) == 0){
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00010111");
            strcat(operacao, "000000000000000000000000");
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else if(strcmp("out", aux->instrucao) == 0){
            char * rd = convertReg(aux->rd);
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00010110");
            strcat(operacao, "000000");
            strcat(operacao, rd);
            strcat(operacao, "000000000000");
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        else{
            char * operacao = (char *) malloc(sizeof(char)*32);
            sprintf(operacao, "00011000");
            strcat(operacao, "000000000000000000000000");
            insereBinInst(criaBinInst(operacao), binCodeList);
        }
        aux = aux->proximo;
    }
}

char * convertReg(char * reg){
    if(strcmp("fp", reg) == 0){
        return("111101");
    }
    else if(strcmp("sp", reg) == 0){
        return("111110");
    }
    else{
        char * temp = reg+1;
        int num = atoi(temp);
        return(toBinaryString(num, 6));
    }
    return NULL;
}

char* toBinaryString(int n, int num_bits) {
    char *string = malloc(num_bits);
    if (!string) {
        return NULL;
    }
    for (int i = num_bits - 1; i >= 0; i--) {
        string[i] = (n & 1) + '0';
        n >>= 1;
    }
    //string[num_bits] = '\0';
    return string;
}

int getLabelLine(char * label, TAssemblyCode * assemblyCodeList){
    TAssemblyInst * aux = assemblyCodeList->inicio;
    int line = 0;
    while (aux != NULL)
    {
        if(strcmp(label, aux->instrucao) == 0){
            return line;
        }
        aux = aux->proximo;
        line++;
    }
    
}