TBinCode * criaBinCode();
TBinInst * criaBinInst(char * instrucao);
void insereBinInst(TBinInst * noInst, TBinCode * code);
void imprimeBinCode(TBinCode * binCodeList);
void geraBinCode(TBinCode * binCodeList, TAssemblyCode * assemblyCodeList);
char * convertReg(char * reg);
char * toBinaryString(int n, int num_bits);
int getLabelLine(char * labe, TAssemblyCode * assemblyCodeList);