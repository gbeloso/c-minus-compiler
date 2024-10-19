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