Tquadruplas * criaQuadrupla();
void insere_inst(char * instrucao, char * op1, char * op2, char * res, Tquadruplas * quadrupla);
void imprimeQuadruplas(Tquadruplas * quadrupla);
char * generateInterCode(TreeNode * t, Tquadruplas * quadrupla);
char * criaLabel();
char * atribuiReg(char * var, char * escopo);
Tregs * criaRegs();
int busca_reg(char * var, char * escopo);
void insere_reg(char * var, char * escopo, int reg);
Tregs * criaRegs();