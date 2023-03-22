%{ 
    #include "util.h"
    #include "globals.h"
    #include "parser.tab.h"
    FILE * fonte;
    FILE * tokens;
    int lineno = 1;
%} 

digit   [0-9] 
number  {digit}+ 
letter  [a-zA-Z] 
identifier  {letter}+ 
newline \n 
whitespace  [ \t \r]+ 
%option noyywrap
%% 

"if"        {fprintf(tokens, "<if, IF>");return IF;};
"else"      {fprintf(tokens, "<else, ELSE>");return ELSE;};
"int"       {fprintf(tokens, "<int, INT>");return INT;};
"return"    {fprintf(tokens, "<return, RETURN>");return RETURN;};
"void"      {fprintf(tokens, "<void, VOID>");return VOID;};
"while"     {fprintf(tokens, "<while, WHILE>");return WHILE;};
"+"         {fprintf(tokens, "<+, PLUS>");return PLUS;};
"-"         {fprintf(tokens, "<-, MINUS>");return MINUS;};
"*"         {fprintf(tokens, "<*, TIMES>");return TIMES;};
"/"         {fprintf(tokens, "</, SLASH>");return SLASH;};
"<"         {fprintf(tokens, "<<, LT>");return LT;};
"<="        {fprintf(tokens, "<<=, LE>");return LE;};
">"         {fprintf(tokens, "<>, GT>");return GT;};
">="        {fprintf(tokens, "<>=, GE>");return GE;};
"=="        {fprintf(tokens, "<==, DEQUAL>");return DEQUAL;};
"!="        {fprintf(tokens, "<!=, DIFFERENT>");return DIFFERENT;};
"="         {fprintf(tokens, "<=, EQUAL>");return EQUAL;};
";"         {fprintf(tokens, "<;, SEMICOLON>");return SEMICOLON;};
","         {fprintf(tokens, "<,, COMMA>");return COMMA;};
"("         {fprintf(tokens, "<(, LPAREN>");return LPAREN;};
")"         {fprintf(tokens, "<), RPAREN>");return RPAREN;};
"["         {fprintf(tokens, "<[, LBRACKET>");return LBRACKET;};
"]"         {fprintf(tokens, "<], RBRACKET>");return RBRACKET;};
"{"         {fprintf(tokens, "<{, LBRACE>");return LBRACE;};
"}"         {fprintf(tokens, "<}, RBRACE>");return RBRACE;};
{number}    {fprintf(tokens, "<%s, NUM>", yytext);return NUM;}; 
{identifier}    {fprintf(tokens, "<%s, ID>", yytext);return ID;};
{newline}       {fprintf(tokens, "\n");lineno++;};
{whitespace}    {fprintf(tokens, "%s", yytext);}; 
"/*"             { char c;
                  while (1) {
                    c = input();
                    if (c == EOF) break;
                    if (c == '\n') lineno++;
                    while ((c = input()) == '*');
                    if (c == '/') break;
                  }
                }
.               {printf("Lexical error at line: %d.\n", lineno);printf("Current token: %s\n", yytext);return ERROR;}

%% 

