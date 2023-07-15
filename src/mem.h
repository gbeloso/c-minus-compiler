enum{Args, Params, Stack, Heap, SP, FP, JumpAddress};

TMemoryList * criaListaMemoria();
TMemory * criaMemoria(char * escopo);
TData * criaData(char * dataName, int size, int memoryPos, int varType, int array);
void imprimeMemoriaLista(TMemoryList * lista);
void imprimeMemoria(TMemory * memoria);
void imprimeData(TData * data);
void insereEscopo(char * escopo);
void insereVar(char * escopo, char * var, int size, int varType, int array);
TData * getData(char * escopo, char * dataName);
TMemoryList * criaMundo();
TData * is_global(char * var);
char * getJumpAddress(char * escopo);
char * getSP(char * escopo);
char * getFP(char * escopo);
TMemory * getEscopo(char * escopo);