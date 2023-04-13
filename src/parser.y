%{
#include <stdio.h>
#include<stdbool.h>
#include<string.h>
#include "globals.h"
#include "util.h"
#include "scan.h"

FILE * arvore;
TreeNode * savedTree;

int yyerror(char * message); // funcao de captura de erros sintaticos
// enum dos tipos de no que a arvore sintatica pode ter
#define YYSTYPE TreeNode * // define o tipo que a funcao yyparse retorna

%}

%token IF ELSE INT RETURN VOID WHILE PLUS MINUS TIMES SLASH LT LE GT GE DEQUAL DIFFERENT EQUAL SEMICOLON COMMA LPAREN RPAREN LBRACKET RBRACKET RBRACE LBRACE NUM ID
%token ERROR 

%%

program : decl_lista {savedTree = $1;};

decl_lista : decl_lista decl {
    // verifica se a lista de declaracoes eh nula, caso seja retorna a ultima declaracao reduzida
    TreeNode * t;
    t = $1;
    if (t == NULL){
        $$ = $2;
    }
    // caso a lista de declaracoes nao seja nula, procura o ultimo irmao (ultima declaracao) e "encaixa" a declaracao atual nela
    else{
        while(t->sibling != NULL){
            t = t->sibling;
        }
        t->sibling = $2;
        $$ = $1;
    }
} 
    | decl{$$ = $1;};

decl : var_decl {$$ = $1;} | fun_decl {$$ = $1;};

fun_decl : tipo id LPAREN params RPAREN composto_decl{
    // cria um no de tipo funcao, aponta o filho esquerdo dele como os parametros e os filhos do meio como sendo as declaracoes da func
    TreeNode * t;
    t = newNode($1->token, $2->lexema, FunNode, $2->lineno);
    t->childL = $4;
    t->childM = $6;
    destroiNo($1);
    destroiNo($2);
    // destroi os nos auxiliares id e tipo recebidos apenas para a construcao do no funcao
    $$ = t;
};

params : param_lista {$$ = $1;} | VOID {$$ = NULL;};

param_lista : param_lista COMMA param 
            {
                // insere o param como irmao da lista de param
                TreeNode * t;
                t = $1;
                if (t == NULL){
                    $$ = $3;
                }
                else{
                        while(t->sibling != NULL){
                            t = t->sibling;
                        }
                    t->sibling = $3;
                    $$ = $1;
                }
            }    
            | param {$$ = $1;};

param : tipo id
        {
            // cria no do tipo parametro
            TreeNode * t;
            t = newNode($1->token, $2->lexema, ParamNode, $2->lineno);
            destroiNo($1);
            destroiNo($2);
            // destroi os nos tipo e id que servem apenas para a construcao do no parametro
            $$ = t;
        }
     | tipo id LBRACKET RBRACKET 
        {
             // cria no do tipo vetor parametro
            TreeNode * t;
            t = newNode($1->token, $2->lexema, VetorParamNode, $2->lineno);
            destroiNo($1);
            destroiNo($2);
            // destroi os nos tipo e id que servem apenas para a construcao do no parametro
            $$ = t;
     };

local_decl : local_decl var_decl {
    //insere a var_decl como irmao de uma local_decl e reduz a uma local_decl
    TreeNode * t;
    t = $1;
    if (t == NULL){
        $$ = $2;
    }
    else{
        while(t->sibling != NULL){
            t = t->sibling;
        }
        t->sibling = $2;
        $$ = $1;
    }
} | {$$ = NULL;};

var_decl : tipo id SEMICOLON {
            //cria o no var_decl
            TreeNode * t;
            t = newNode($1->token, $2->lexema, Decl_varNode, $2->lineno);
            destroiNo($1);
            destroiNo($2);
            // destroi os nos auxiliares tipo e id
            $$ = t;
}       
        | tipo id LBRACKET num RBRACKET SEMICOLON{
            // cria o no do tipo Decl_vetorNode
            TreeNode * t;
            t = newNode($1->token, $2->lexema, Decl_vetorNode, $2->lineno);
            t->childL = $4;
            destroiNo($1);
            destroiNo($2);
            // destroi os nos auxiliares tipo e id
            $$ = t;
};

num : NUM {
    // cria o no NUM
    TreeNode * t;
    t = newNode(NUM, yytext, NumNode, lineno);
    t->token = INT; 
    $$ = t;
};

tipo : INT {
        // cria o no auxiliar int;
        TreeNode * t;
        t = newNode(INT,"int",0, lineno);
        $$ = t;
} 
    | VOID{
        // cria o no auxiliar void
        TreeNode * t;
        t = newNode(VOID,"void",0, lineno);
        $$ = t;
};

statement_lista : statement_lista statement {
                    // anexa o no statement como irmao do no statement_lista
                    TreeNode * t;
                    t = $1;
                    if (t == NULL){
                        $$ = $2;
                    }
                    else{
                        while(t->sibling != NULL){
                            t = t->sibling;
                        }
                        t->sibling = $2;
                        $$ = $1;
                    }
} | {$$ = NULL;} ;

statement : exp_decl {$$ = $1;} | retorno_decl {$$ = $1;} | iteracao_decl {$$ = $1;} | selecao_decl {$$ = $1;} | composto_decl {$$ = $1;};

composto_decl : LBRACE local_decl statement_lista RBRACE {
    //insere a statement lista como irmao da local_decl e reduz a um composto_decl
    TreeNode * t;
    t = $2;
    if(t == NULL){
        $$ = $3;
    }
    else{
        while(t->sibling != NULL){
            t = t->sibling;
        }
        t->sibling = $3;
        $$ = $2;
    }
};

selecao_decl : IF LPAREN exp RPAREN statement{
                // cria o no do tipo if e aloca exp como filho a esquerda e statement como filho do meio
                TreeNode * p;
                TreeNode * fE;
                TreeNode * fM;
                p = newNode(IF, "if", IfNode, $3->lineno);
                fE = $3;
                fM = $5;
                p->childL = fE;
                p->childM = fM;
                $$ = p;
} | IF LPAREN exp RPAREN statement ELSE statement{
                // cria o no do tipo if e aloca exp como filho a esquerda, statement como filho do meio e o statement do else como filho da direita
                TreeNode * p;
                TreeNode * fE;
                TreeNode * fM;
                TreeNode * fD;
                p = newNode(IF, "if", IfNode, $3->lineno);
                fE = $3;
                fM = $5;
                fD = $7;
                p->childL = fE;
                p->childM = fM;
                p->childR = fD;
                $$ = p;
};

iteracao_decl : WHILE LPAREN exp RPAREN statement{
                // cria o no while e aloca exp como filho a esquerda e statement como filho do meio
                TreeNode * p;
                TreeNode * fE;
                TreeNode * fM;
                p = newNode(WHILE, "while", WhileNode, $3->lineno);
                fE = $3;
                fM = $5;
                p->childL = fE;
                p->childM = fM;
                $$ = p;
};

retorno_decl : RETURN SEMICOLON {$$ = NULL;} | RETURN exp SEMICOLON {
                // cria no do tipo return e aloca exp como filho a esquerda
                TreeNode * p;
                TreeNode * fE;
                p = newNode(RETURN, "return", ReturnNode, $2->lineno);
                fE = $2;
                p->childL = fE;
                $$ = p;
};

exp_decl : exp SEMICOLON {$$ = $1;} | SEMICOLON {$$ = NULL;};

exp : var EQUAL exp{
    // cria o no exp aloca var como filho a esquerda e exp como filho a direita
    TreeNode * p;
    TreeNode * fE;
    TreeNode * fM;
    p = newNode(EQUAL, "=", OpNode, $1->lineno);
    fE = $1;
    fM = $3;
    p->childL = fE;
    p->childM = fM;
    $$ = p;
} 
    | simples_exp {$$ = $1;};

simples_exp : soma_exp relacional soma_exp {
    //cria o no simples_exp e atribui como filho da esquerda e direita um soma_exp
    TreeNode * t;
    TreeNode * fE;
    TreeNode * fM;
    t = newNode($2->token, $2->lexema, OpNode, $2->lineno);
    fE = $1;
    fM = $3;
    t->childL = fE;
    t->childM = fM;
    destroiNo($2);
    //destroi o no auxiliar relacional
    $$ = t;
} 
            | soma_exp {$$ = $1;};

relacional : LE {
            // cria os nos auxiliares relacionas <= < > >= == !=
            TreeNode * t;
            t = newNode(LE, yytext, OpNode, lineno);
            $$ = t;
            } 
            | LT {    
            TreeNode * t;
            t = newNode(LT, yytext, OpNode, lineno);
            $$ = t;
            } 
            | GT{
            TreeNode * t;
            t = newNode(GT, yytext, OpNode, lineno);
            $$ = t;
            } 
            | GE {
            TreeNode * t;
            t = newNode(GE, yytext, OpNode, lineno);
            $$ = t;
            }
            | DEQUAL {
            TreeNode * t;
            t = newNode(DEQUAL, yytext, OpNode, lineno);
            $$ = t;
            } 
            | DIFFERENT{
            TreeNode * t;
            t = newNode(DIFFERENT, yytext, OpNode, lineno);
            $$ = t;
            };

soma_exp : soma_exp soma termo {
    //cria o no soma exp e atribui soma_exp como filho a esquerda e termo como filho a direita
    TreeNode * t;
    TreeNode * fE;
    TreeNode * fM;
    t = newNode($2->token, $2->lexema, OpNode, $2->lineno);
    fE = $1;
    fM = $3;
    t->childL = fE;
    t->childM = fM;
    destroiNo($2);
    //destroi o no auxiliar soma
    $$ = t;
} | termo {$$ = $1;};

soma : PLUS {
        //cria os nos auxiliares de soma e subtracao
        TreeNode * t;
        t = newNode(PLUS, yytext, OpNode, lineno);
        $$ = t;
} 
    | MINUS{
        TreeNode * t;
        t = newNode(MINUS, yytext, OpNode, lineno);
        $$ = t;
    };

termo: termo mult fator {
    //cria o no termo e atribui como filho a esquerda um termo e um fator como filho a direita
    TreeNode * t;
    TreeNode * fE;
    TreeNode * fM;
    t = newNode($2->token, $2->lexema, OpNode, $2->lineno);
    fE = $1;
    fM = $3;
    t->childL = fE;
    t->childM = fM;
    destroiNo($2);
    //destroi o no auxiliar mult
    $$ = t;
} | fator {$$ = $1;};

mult: TIMES {
        // constroi os nos auxiliares de multiplicacao e divisao
        TreeNode * t;
        t = newNode(TIMES, yytext, OpNode, lineno);
        $$ = t;
} 
    | SLASH{
        TreeNode * t;
        t = newNode(SLASH, yytext, OpNode, lineno);
        $$ = t;
    };

fator : LPAREN exp RPAREN {$$ = $2;} 
        | var {$$ = $1;} 
        | ativacao {$$ = $1;} 
        | num  {$$ = $1;};

var : id {
    //cria o no do tipo var
    TreeNode *t;
    t = newNode(ID, $1->lexema, VarNode, $1->lineno);
    destroiNo($1);
    //detroi o no auxiliar id
    t->token = INT;
    $$ = t;
}   | id LBRACKET exp RBRACKET{
        // cria no do tipo vetor e atribui exp como filho a esquerda
        TreeNode *t;
        TreeNode * fE;
        t = newNode($1->token, $1->lexema, VetorNode, $1->lineno);
        fE = $3;
        t->childL = fE;
        destroiNo($1);
        // destroi no auxiliar id
        t->token = INT;
        $$ = t;
};

id : ID {
        //cria o no auxiliar id
        TreeNode * t;
        t = newNode(ID, yytext, 0, lineno);
        $$ = t;
};

ativacao : id LPAREN args RPAREN {
    //cria o no ativacao e aloca args como filho a esquerda
    TreeNode *t;
    TreeNode *fE;
    t = newNode(ID, $1->lexema, AtivNode, $1->lineno);
    destroiNo($1);
    fE = $3;
    t->childL = fE;
    $$ = t;
};

args : arg_lista {$$ = $1;} | {$$ = NULL;};

arg_lista : arg_lista COMMA exp {
    //aloca exp como um irmao de uma lista de argumentos
    TreeNode * t;
    t = $1;
    if (t == NULL){
        $$ = $3;
    }
    else{
        while(t->sibling != NULL){
            t = t->sibling;
        }
        t->sibling = $3;
        $$ = $1;
    }
} | exp {$$ = $1;};

%%

int yyerror(char * message)
{ printf("Syntax error at line %d: %s\n",lineno,message);
  printf("Current token: %s\n", yytext);
  printTree(savedTree, 0);
  return 0;
} //printa a mensagem de erro ao encontrar um token nao esperado ou errado;
