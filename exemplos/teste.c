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

int a;

int main(void){
    a = 0;
    a = a + 1;
    output(a * 3 - 4);
}
