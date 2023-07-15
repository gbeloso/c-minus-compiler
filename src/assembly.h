int geraAssembly(Tquadruplas * quadruplas, TAssemblyCode * assemblyList);
TAssemblyCode * criaAssemblyCode();
void insereAssemblyInst(char * instrucao, char * rd, char * rm, char * rn, char * imm, TAssemblyCode * assemblyList);
void imprimeAssembly(TAssemblyCode * assemblyList);
void insereInicioAssemblyInst(char * instrucao, char * rd, char * rm, char * rn, char * imm, TAssemblyCode * assemblyList);