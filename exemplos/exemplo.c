/*base do SO*/

int x[3]; /*Vetor dos programas*/
int y[3]; /*Vetor dos PCs dos programas*/
int z[3]; /*Vetor dos PCs finais dos programas*/
int a; /*Variável temporária*/
int size;

void run(int process, int tipo){
    int tmp;
    int b;
    b = 0;
    tmp = 300;
    if(tipo == 1){
        while(b <= 63){/*salva estado do SO*/
            output(b);
            b = b + 1;
        }
        tmp;
        b = 0;
        while(b <= 63){/*loada estado do SO*/
            output(b);
            b = b + 1;
        }
    }
    return;
}

void main(void) {
    while(1){
        a = input();
	    if(a == 1){
            x[1] = input();
            run(x[1], a);
	    }
    }
}