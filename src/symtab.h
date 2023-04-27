
enum {Function, Variable, Parameter, ArrayVariable, ArrayParameter};

/*typedef struct no
{
    int linhas;
    char * lexema;
    char * escopo;
    int tipo;
    struct no * proximo;
}Tno;

typedef struct lista
{
    Tno * inicio;
    int tam;
}Tlista;*/

Tlista * criaLista();
void insere(int linhas, char * lexema, char * escopo, int tipo_id, int tipo_node, Tlista * linha);
void deleta(Tno * no, Tlista * linha);
Tno * buscar(char * lexema, char * escopo, int tipo_node, Tno * inicio);
void imprime(Tlista * linha);
int hash(char * nome, char * escopo);
int verificaTipoNode(int t1, int t2);
