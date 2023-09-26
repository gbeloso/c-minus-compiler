/* Potencia */
int pot(int base, int expoente){
    int res;
    res = 1;
    while(expoente > 0){
        res = res * base;
        expoente = expoente - 1;
    }
    return(res);
}

int main(void){
    int base;
    int expoente;
    input(base);
    input(expoente);
    output(pot(base, expoente));
    return 0;
}