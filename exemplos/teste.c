/* Um programa para ordenação por seleção de uma matriz com dez elementos. */

int x[10];

int minloc (int a[], int low, int high) { 
    int i; 
    int j; 
    int k; 
    k = low; 
    j = a[low];
    i = low + 1; 
    while (i < high) { 
        if (a[i] < j) { 
            j = a[i]; 
            k = i; 
        } 
        i = i + 1;
    } 
    return k; 
}

void sort(int a[], int low, int high) { 
    int i; 
    int k; 
    i = low;
    while (i < high-1) { 
        int t;
        k = minloc(a,i,high); 
        t = a[k]; 
        a[k] = a[i]; 
        a[i] = t; 
        i = i + 1;
    } 
}

void main(void) { 
    int i; 
    i = 0;
    while (i < 10) { 
        x[i] = input(); 
        i = i + 1; 
    }
    sort(x,0,10); 
    i = 0;
    while (i < 10) { 
        output(x[i]); 
        i = i + 1; 
    }
}

/* Um programa para calcular o mdc segundo o algoritmo de Euclides. */

int gcd (int u, int v) { 
    if (v == 0) return u ; 
    else return gcd(v,u-u/v*v); /* u-u/v*v == u mod v */
}

void main(void) { 
    int x; 
    int y;
    x = input(); 
    y = input(); 
    output(gcd(x,y));
}

/* área do triangulo */
int area(int b, int h){
    return(b*h/2);
}

int main(void){
    int h;
    int b;
    h = input();
    b = input();
    output(area(b, h));
}

/* Fibonacci*/
int fib(int n){
    if(n == 0){
        return 0;
    }
    else if (n == 1)
    {
        return 1;
    }
    else{
        int k;
        int fn;
        int fnMenosUm;
        int res;
        fn = 1;
        fnMenosUm = 0;
        k = 0;
        while(k < n - 1){
            res = fn + fnMenosUm;
            fnMenosUm = fn;
            fn = res;
            k = k + 1;
        }
        return res;
    }
}

int main(void){
    int n;
    n = input();
    output(fib(n));
}

/* Fatorial*/
int fat(int n){
    if(n == 0){
        return(1);
    }
    else{
        return(n*fat(n-1));
    }
}

int main(void){
    int n;
    n = input();
    output(fat(n));
}

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
    base = input();
    expoente = input();
    output(pot(base, expoente));
    return 0;
}