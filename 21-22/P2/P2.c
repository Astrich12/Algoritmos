/*
Julian Blanco Sóñora
Xián Vázquez Luna
Alvaro Sangil Turnes
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <math.h>

void inicializar_semilla() {
    srand(time(NULL));
}

void aleatorio(int v [], int n) {
/* se generan números pseudoaleatorio entre -n y +n */
    int i, m=2*n+1;
    for (i=0; i < n; i++)
    v[i] = (rand() % m) - n;
}

double microsegundos() {    
/* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void ascendente(int v [], int n) {
    int i;
    for (i=0; i < n; i++)
        v[i] = i;
}

void descendente(int v [], int n) {
    int i,num = n;
    for (i=0; i < n; i++){
        v[i] = num;
        num -= 1;
    }
}

void listar_vector(int v[],int n){
    int i;
    printf("[ ");
    for (i = 0;i<n;i++){
        printf("%4d",v[i]);
    }
    printf(" ] \n");
}

void ord_sel(int v[], int n){
    int i, j, minj, minx;
    for(i=0;i<n;i++){
        minj = i;
        minx = v[i];
        for(j=i+1;j<n;j++){
            if(v[j]<minx){
                minj=j;
                minx=v[j];		
            }
        }
        v[minj] = v[i];
        v[i] = minx;
    }
}

void ord_shell (int v [], int n){

    int incremento = n, i, tmp, j;
    bool seguir;
    while (incremento != 1){
        incremento = incremento / 2;
        for (i = incremento; i < n; i++){
            tmp = v[i];
            j = i;
            seguir = true;
            while ((j - incremento >= 0)&&(seguir)){
                if (tmp < v[j - incremento]){
                    v[j] = v[j - incremento];
                    j = j - incremento;
                }else
                    seguir = false;
            }
            v[j] = tmp;
        }
    }
}

void test_seleccion(){
    int vector[25];
    printf("Test algoritmo ordenación:\n");
    printf("\tVector ordenado:\n");
    ascendente(vector,25);
    listar_vector(vector,25);
    ord_sel(vector,25);
    printf("\tDespués de aplicar el algoritmo:\n");
    listar_vector(vector,25);
    printf("\n");

    printf("\tVector ordenado al revés:\n");
    descendente(vector,25);
    listar_vector(vector,25);
    ord_sel(vector,25);
    printf("\tDespués de aplicar el algoritmo:\n");
    listar_vector(vector,25);
    printf("\n");

    printf("\tVector aleatorio:\n");
    aleatorio(vector,25);
    listar_vector(vector,25);
    ord_sel(vector,25);
    printf("\tDespués de aplicar el algoritmo:\n");
    listar_vector(vector,25);
    printf("\n");
}


void test_shell(){
    int vector[25];
    printf("Test algoritmo shell:\n");
    printf("\tVector ordenado:\n");
    ascendente(vector,25);
    listar_vector(vector,25);
    ord_shell(vector,25);
    printf("\tDespués de aplicar el algoritmo:\n");
    listar_vector(vector,25);
    printf("\n");

    printf("\tVector ordenado al revés:\n");
    descendente(vector,25);
    listar_vector(vector,25);
    ord_shell(vector,25);
    printf("\tDespués de aplicar el algoritmo:\n");
    listar_vector(vector,25);
    printf("\n");

    printf("\tVector aleatorio:\n");
    aleatorio(vector,25);
    listar_vector(vector,25);
    ord_shell(vector,25);
    printf("\tDespués de aplicar el algoritmo:\n");
    listar_vector(vector,25);
    printf("\n");
}

double CalcTiempos(void (*algoritmo)(int v[], int tam), int tam,
                    void(*orden)(int v[], int tam), int k){
    double t_start=0.0; //tiempo de inicio
    double t_fin=0.0; //tiempo de fin
    double t=0.0; //tiempo del test
    int i;
    int *v;

    v = malloc(tam*sizeof(int));
    orden(v, tam);
    t_start = microsegundos();
    algoritmo(v, tam);
    t_fin = microsegundos();
    t = t_fin - t_start;

    if(t<500){
        printf("(*)");
        t_start = microsegundos();
        for(i=0;i<k;i++){
            orden(v,tam);
            algoritmo(v,tam);
        }
        t_fin = microsegundos();
        t = (t_fin - t_start);
        t_start = microsegundos();
        for(i=0;i<k;i++)
            orden(v,tam);
        t_fin = microsegundos();
        t = (t - (t_fin - t_start))/k;
    }
    free(v);
    return t;
}

void print_sel(){
    int n,k=100;
    double tiempo;

    printf("ORDENACION POR SELECCION\n\n");

    printf("VECTORES INICIADOS ASCENDENTEMENTE:\n");
    printf("\t      n\t\t\t\t  t(n)\t\t    t(n)/n^1.80\t\t  "
            "  t(n)/n^2.00\t\t    t(n)/n^2.20\n");
    for(n=500;n<=32000;n*=2){
        tiempo = CalcTiempos(ord_sel, n, ascendente, k);
        printf("\t% 8d\t\t% 14.5f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n", n,
                tiempo, tiempo/(pow(n,1.8)), tiempo/(pow(n,2)), 
                tiempo/(pow(n,2.2)));
    }
    printf("\n");
    
    printf("VECTORES INICIADOS DESCENDENTEMENTE:\n");
    printf("\t      n\t\t\t\t  t(n)\t\t    t(n)/n^1.80\t\t  "
            "  t(n)/n^2.00\t\t    t(n)/n^2.20\n");
    for(n=500;n<=32000;n*=2){
        tiempo = CalcTiempos(ord_sel, n, descendente, k);
        printf("\t% 8d\t\t% 14.5f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n", n,
                tiempo, tiempo/(pow(n,1.8)), tiempo/(pow(n,2)), 
                tiempo/(pow(n,2.2)));
    }
    printf("\n");

    printf("VECTORES INICIADOS ALEATORIAMENTE:\n");
    printf("\t      n\t\t\t\t  t(n)\t\t    t(n)/n^1.80\t\t  "
            "  t(n)/n^2.00\t\t    t(n)/n^2.20\n");
    for(n=500;n<=32000;n*=2){
        tiempo = CalcTiempos(ord_sel, n, aleatorio, k);
        printf("\t% 8d\t\t% 14.5f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n", n,
                tiempo, tiempo/(pow(n,1.8)), tiempo/(pow(n,2)), 
                tiempo/(pow(n,2.2)));
    }
    printf("\n");  
}

void print_shell(){
    int n,k=100;
    double tiempo=0.0;
    printf("ORDENACION POR SHELL\n\n");
    printf("VECTORES INICIADOS ASCENDENTEMENTE:\n");
    printf("\t      n\t\t\t\t  t(n)\t\t    t(n)/n^0.09\t\t "
            "   t(n)/n^1.10\t\t    t(n)/n^1.20\n");
    for(n=500;n<=32000;n*=2){
        tiempo = CalcTiempos(ord_shell, n, ascendente, k);
        printf("\t% 8d\t\t% 14.5f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n", n,
                tiempo, tiempo/(pow(n,0.9)), tiempo/(pow(n,1.10)),
                tiempo/(pow(n,1.2)));
    }
    printf("\n");
    
    printf("VECTORES INICIADOS DESCENDENTEMENTE:\n");
    printf("\t      n\t\t\t\t  t(n)\t\t    t(n)/n^0.95\t\t  "
            "  t(n)/n^1.15\t\t    t(n)/n^1.38\n");
    for(n=500;n<=32000;n*=2){
        tiempo = CalcTiempos(ord_shell, n, descendente, k);
        printf("\t% 8d\t\t% 14.5f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n", n,
                tiempo, tiempo/(pow(n,0.95)), tiempo/(pow(n,1.15)),
                tiempo/(pow(n,1.38)));
    }
    printf("\n");
    
    printf("VECTORES INICIADOS ALEATORIAMENTE:\n");
    printf("\t      n\t\t\t\t  t(n)\t\t    t(n)/n^1.12\t\t  "
            "  t(n)/n^1.20\t\t    t(n)/n^1.30\n");
    for(n=500;n<=32000;n*=2){
        tiempo = CalcTiempos(ord_shell, n, aleatorio, k);
        printf("\t% 8d\t\t% 14.5f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n", n,
                tiempo, tiempo/(pow(n,1.12)), tiempo/(pow(n,1.20)), 
                tiempo/(pow(n,1.3)));
    }
    printf("\n");

}

int main(){
    int i;
    inicializar_semilla();
    //test_seleccion();
    //test_shell();
    for(i=0;i<3;i++){
        print_sel();
        print_shell();
    }

    return 0;
}
