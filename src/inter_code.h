Tquadruplas * criaQuadrupla();
void insere_inst(char * instrucao, char * op1, char * op2, char * res, Tquadruplas * quadrupla);
void deleta_inst(Tinst * inst, Tquadruplas * quadrupla);
Tno * busca_op(char * op, Tinst * inicio);
void imprimeQuadruplas(Tquadruplas * quadrupla);
char * generateInterCode(TreeNode * t, Tquadruplas * quadrupla);
char * criaLabel();
char * atribuiReg();