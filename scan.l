%{ 
    FILE * fonte;
    FILE * tokens;
    int lineno = 1;
    enum {IF, ELSE, INT, RETURN, VOID, WHILE, PLUS, MINUS, TIMES, SLASH, LT, LE, GT, GE, DEQUAL, DIFFERENT, EQUAL, SEMICOLON, COMMA, LPAREN, RPAREN, LBRACKET, RBRACKET, RBRACE, LBRACE, NUM, ID, YYEOF, ERROR};
%} 
digit   [0-9] 
number  {digit}+ 
letter  [a-zA-Z] 
identifier  {letter}+ 
newline \n 
whitespace  [ \t]+ 
%% 

“if”        {fprintf(tokens, "<if, IF>");return IF;};
“else”      {fprintf(tokens, "<else, ELSE>");return ELSE;};
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
.               {return ERROR;}

%% 

void main(int arcg, char ** argv){
    int temp = 1;
    fonte = fopen("exemplo.c", "r");
    tokens = fopen("tokens.txt", "w");
    yyin = fonte;
    while(temp != 0){
        temp = yylex();
        printf("%d\n", temp);
    }
}