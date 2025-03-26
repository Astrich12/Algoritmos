#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void inicializar_semilla() {
    srand(time(NULL));
    /* se establece la semilla de una nueva serie de enteros pseudo-aleatorios */
}
void aleatorio(int v [], int n) {
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
    /* se generan números pseudoaleatorio entre -n y +n */
}

double microsegundos() { /* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void listar_vector(int v[],int n){
    int i;
    printf("[ ");
    for (i = 0;i<n;i++){
        printf("%4d",v[i]);
    }
    printf(" ] ");
}

int sumaSubMax1(int v[], int tam){
    int i, j;
    int sumaMax = 0, estaSuma = 0;
    for(i=0;i<tam;i++){
        estaSuma = 0;
        for (j=i;j<tam;j++){
            estaSuma += v[j];
            if(estaSuma > sumaMax)
                sumaMax = estaSuma;
        }
    }
    return sumaMax;
}

int sumaSubMax2(int v[], int tam){
    int j;
    int estaSuma=0, sumaMax=0;
    for (j=0;j<tam;j++){
        estaSuma += v[j];
        if(estaSuma > sumaMax)
            sumaMax = estaSuma;
        else if (estaSuma < 0)
            estaSuma = 0;
    }
    return sumaMax;
}

void test1(){
    int v[6][5] = {{-9,2,-5,-4,6},{4,0,9,2,5},{-2,-1,-9,-7,-1},
                   {9,-2,1,-7,-8},{15,-2,-5,-4,16},{7,-5,6,7,-7}};
    int i,a,b;
    printf("test1\n");
    printf("%22s%15s%15s\n", "", "sumaSubMax1", "sumaSubMax2");
    for (i=0; i<6; i++) {
        listar_vector(v[i], 5);
        a = sumaSubMax1(v[i], 5);
        b = sumaSubMax2(v[i], 5);
        printf("%8d%15d\n", a, b);
    }
    printf("\n");
}

void test2() {
    int i, a, b;
    int v[9];
    printf("test2\n");
    printf("%38s%15s%15s\n", "", "sumaSubMax1", "sumaSubMax2");
    for (i=0; i<10; i++) {
        aleatorio(v, 9);
        listar_vector(v, 9);
        a = sumaSubMax1(v, 9);
        b = sumaSubMax2(v, 9);
        printf("%8d%15d\n", a, b);
    }
    printf("\n");
}

double CalcTiempos(int (*algoritmo)(int v[], int tam), int tam, int k){
    double t_start; //tiempo de inicio
    double t_fin; //tiempo de fin
    double t; //tiempo del test
    int i;
    int *v;

    v = malloc(tam*sizeof(int));
    aleatorio(v, tam);
    t_start = microsegundos();
    algoritmo(v, tam);
    t_fin = microsegundos();
    t = t_fin - t_start;

    if(t<500){
        printf("(*)");
        t_start = microsegundos();
        for(i=0;i<k;i++){
            aleatorio(v,tam);
            algoritmo(v,tam);
        }
        t_fin = microsegundos();
        t = (t_fin - t_start);
        t_start = microsegundos();
        for(i=0;i<k;i++){
            aleatorio(v,tam);
        }
        t_fin = microsegundos();
        t = (t - (t_fin - t_start))/k;
    }
    free(v);
    return t;
}
void test3(){
    int n,k=100,i;
    double tiempo;
        //Algoritmo 1
    printf("sumaSubMax1\n");
    printf("\t      n\t\t\t\t  t(n)\t\t     t(n)/n^1.8\t\t  "
           "   t(n)/n^2.0\t\t     t(n)/n^2.2\n");
    for(i=0;i<5;i++){
        for(n=500;n<=32000;n*=2){
        tiempo = CalcTiempos(sumaSubMax1, n, k);
        printf("\t% 8d\t\t% 14.5f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n", n,
               tiempo, tiempo/(pow(n,1.8)), tiempo/(pow(n,2)), tiempo/(pow(n,2.2)));
        }
        printf("\n");
            //Algoritmo 2
        printf("sumaSubMax2\n");
        printf("\t      n\t\t\t\t  t(n)\t\t     t(n)/n^0.8\t\t  "
                "   t(n)/n\t\t     t(n)/n^1.2\n");
        for (n=500; n<=32000; n*=2){
            tiempo = CalcTiempos(sumaSubMax2, n, k);
            printf("\t% 8d\t\t% 14.5f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n",
                    n, tiempo, tiempo/(pow(n,0.8)), tiempo/n, tiempo/(pow(n,1.2)));
        }
        printf("\n");
        printf("\n\n (*) Tiempo promedio en %d ejecuciones del algoritmo\n\n",k);
    }
}
    

int main() {
    inicializar_semilla();
    test1();
    test2();
    test3();
    return 0;
}
