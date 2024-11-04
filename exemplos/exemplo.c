/* Soma escalar FUNCIONOU*/
int somaEscalar(int x){
	int resultado;
	resultado = x + 5;
	return resultado;
}

int main(void){
	int x; int y; int resposta;
	x = input();
	resposta = somaEscalar(x);
	output(resposta);
}