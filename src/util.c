#include "util.h"
#include "globals.h"

char * copiaString(char * palavra){
    int n;
    char * t;
    if (palavra == NULL) return NULL;
    n = strlen(palavra)+1;
    t = malloc(n);
    if (t == NULL)
        printf("Out of memory error at line %d\n",lineno);
    else strcpy(t,palavra);
    return t;
}