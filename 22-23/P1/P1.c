		/*PRACTICA 1*/
/* Nombre 
	Julián Blanco Sóñora
	Juan Veiguela Paz
    Anxo Mosquera Bello
*/

/*Login
	julian.blanco.sonora@udc.es
	juan.veiguela@udc.es
    anxo.mosquera.bello@udc.es
*/



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

double microsegundos() {  /* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

int fib1(int n) {
    if (n < 2) return n;
    else return (fib1(n-1) + fib1(n-2));
}

int fib2(int n) {
    int i = 1, j = 0;
    for (int k = 0; k < n; k++){
        j += i;
        i = j - i;
    }
    return j;
}

int fib3(int n) {
    int i = 1, j = 0, k = 0, h = 1, t = 0;
    while (n > 0){
        if ((n % 2) != 0){
            t = j*h;
            j = i*h + j*k + t;
            i = i*k + t;
            n -= 1;
        } else {
            t = h*h;
            h = 2*k*h + t;
            k = k*k + t;
            n = n/2;
        }
    }
    return j;
}

void ascendente(int v [], int n) {
    int i;
    for (i=0; i < n; i++)
    v[i] = i;
}

void test_funcionamiento(int (*funcion)(int n)){
    int n = 10, v[n];
    ascendente(v,n);

    for(int i = 0; i < n; i++){
        printf("%d ",funcion(v[i]));
    }
    
    printf("\n");
}

double CalcTiempos(int (*algoritmo)(int tam), int tam,
                    int k){
    double t_start=0.0; //tiempo de inicio
    double t_fin=0.0; //tiempo de fin
    double t=0.0; //tiempo del test
    int i;

    t_start = microsegundos();
    algoritmo(tam);
    t_fin = microsegundos();
    t = t_fin - t_start;

    if(t<500){
        printf("(*)");
        t_start = microsegundos();
        for(i=0;i<k;i++){
            algoritmo(tam);
        }
        t_fin = microsegundos();
        t = (t_fin - t_start)/k;
    }
    return t;
}

void estudio_fib(){
    int n,k=100000;
    double tiempo;

    printf("EJECUCION FIB1\n\n");

    printf("\t       n\t\t\t t(n)\t\t     t(n)/1.1^n\t\t"
            "t(n)/((1+sqrt(5)/2)^n)\t       t(n)/2^n\n");
    for(n=2;n<=32;n*=2){
        tiempo = CalcTiempos(fib1, n, k);
        printf("\t% 8d\t\t %14.8f\t\t %14.8f\t\t %14.8f\t\t %14.8f\n", n,
                tiempo, tiempo/(pow(1.1,n)), tiempo/pow(((1+sqrt(5))/2),n), 
                tiempo/(pow(2,n)));
    }
    printf("\n");



    
}

void estudio_fib2_3(){
	int n,k=1000;
    double tiempo;

    printf("EJECUCION FIB2\n\n");

    printf("\t       n\t\t\t t(n)\t\t     t(n)/n^0.8\t\t\t "
            "t(n)/n\t\t  t(n)/n*log(n)\n");
    for(n=1000;n<=10000000;n*=10){
        tiempo = CalcTiempos(fib2, n, k);
        printf("\t% 10d\t\t %14.8f\t\t %14.8f\t\t %14.8f\t\t %14.8f\n", n,
                tiempo, tiempo/(pow(n,0.8)), tiempo/n, 
                tiempo/(n*log(n)));
    }
    printf("\n");
    k=100000;
    printf("EJECUCION FIB3\n\n");

    printf("\t       n\t\t\t t(n)\t\t  t(n)/sqrt(log(n))\t  "
            "  t(n)/log(n)\t\t     t(n)/n^0.5\n");
    for(n=1000;n<=10000000;n*=10){
        tiempo = CalcTiempos(fib3, n, k);
        printf("\t% 10d\t\t %14.8f\t\t %14.8f\t\t %14.8f\t\t %14.8f\n", n,
                tiempo, tiempo/(sqrt(log(n))), tiempo/(log(n)), 
                tiempo/(pow(n,0.5)));
    }
    printf("\n");
}

int main(){
    int i;
    printf("\nTEST FIB1\n");
    test_funcionamiento(fib1);
    printf("\nTEST FIB2\n");
    test_funcionamiento(fib2);
    printf("\nTEST FIB3\n");
    test_funcionamiento(fib3);

    for(i=0;i<3;i++){
        printf("\n\n");
        printf("EJECUCION ALGORITMOS\n\n\n");
        estudio_fib();
        estudio_fib2_3();
    }

    return 0;
}