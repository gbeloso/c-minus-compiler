int numProgramas;
int progInicio[3];

void dispatch(int endereco){
    return;
}

void main(void){
    int opcao;
    int escolha;
    int i;

    numProgramas = 3;
    i = 0;
    while(i < numProgramas){
        progInicio[i] = 300 * i + 300;
        i = i + 1;
    }

    while(1){
        output(1);
        opcao = input();

        if(opcao == 1){
            output(numProgramas);
            escolha = input();

            if(escolha >= 0){
                if(escolha < numProgramas){
                    dispatch(progInicio[escolha]);
                }
            }
        }
    }
}
