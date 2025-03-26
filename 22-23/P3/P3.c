                /*PRACTICA 3*/
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
#include <stdbool.h>
#include <math.h>

struct nodo {
    int elem;
    int num_repeticiones;
    struct nodo *izq, *der;
};

typedef struct nodo *posicion;
typedef struct nodo *arbol;

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
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

static struct nodo *crearnodo(int e) {
    struct nodo *p = malloc(sizeof(struct nodo));
    if (p == NULL) {
        printf("Memoria agotada\n");
        exit(EXIT_FAILURE);
    }
    p->elem = e;
    p->num_repeticiones = 1;
    p->izq = NULL;
    p->der = NULL;
    return p;
}

arbol insertar(int e, arbol a){
    if (a == NULL)
        return crearnodo(e);
    else if (e < a->elem)
        a->izq = insertar(e, a->izq);
    else if (e > a->elem)
        a->der = insertar(e, a->der);
    else
        a->num_repeticiones++;
    return a;
}

arbol creararbol(){
    arbol a = NULL;
    return a;
}

int esarbolvacio(arbol a){
    if(a == NULL)
        return 0;
    else
        return -1;
}

posicion buscar(int e, arbol a){

    if (a == NULL){
        return NULL;
    }else{
        if (e == a->elem){
            return a;
        }else if (e < a->elem){
            return buscar (e, a->izq);
        }else if (e > a->elem){
            return buscar (e, a->der);
        }
    }
    return NULL;
}

arbol eliminararbol(arbol a){

    if(a != NULL){
        eliminararbol(a->izq);
        eliminararbol(a->der);
        free(a);
    }
    a = creararbol();
    return a;
}

posicion hijoizquierdo(arbol a){
    posicion pos = a->izq;
    return pos;
}

posicion hijoderecho(arbol a){
    posicion pos = a->der;
    return pos;
}

int elemento(posicion pos){
    int dato = pos->elem;
    return dato;
}

int numerorepeticiones(posicion pos){
    return pos->num_repeticiones;
}


int altura(arbol a){
    int AltIzq, AltDer;
    if(a == NULL)
        return -1;
    else{
        AltIzq = altura(hijoizquierdo(a));
        AltDer = altura(hijoderecho(a));
        if(AltIzq>AltDer)
            return AltIzq+1;
        else
            return AltDer+1;
    }
}

void visualizar(arbol a){
    if (a != NULL){
        if(hijoizquierdo(a) != NULL)
            visualizar (hijoizquierdo(a));
        else
            printf("(");

        if(hijoderecho(a) != NULL && hijoizquierdo(a) != NULL)
            printf(") %d (", elemento(a));
        else
            printf (" %d ", elemento(a));

        if(hijoderecho(a) != NULL)
            visualizar(a->der);
        else
            printf(")");

    }
}

void prueba(){
    int i;
    posicion pos;
    arbol arb = creararbol();
    if (esarbolvacio(arb) == 0)
        printf("El arbol está vacío\n");
    else
        printf("El arbol contiene elementos\n");
    printf("altura del arbol: %d\n",altura(arb));
    printf("Inserto un 3\n");
    arb = insertar(3, arb);
    printf("Inserto un 1\n");
    arb = insertar(1, arb);
    printf("Inserto un 2\n");
    arb = insertar(2, arb);
    printf("Inserto un 5\n");
    arb = insertar(5, arb);
    printf("Inserto un 4\n");
    arb = insertar(4, arb);
    printf("Inserto un 5\n");
    arb = insertar(5, arb);
    visualizar(arb);
    printf("\n");
    printf("Altura del arbol: %d\n", altura(arb));
    for(i=1; i<=6;i++){
        pos = buscar(i, arb);
        if(pos!=NULL)
            printf("Busco %d y encuentro %d repetido: %d veces\n",i, i, 
                    numerorepeticiones(pos));
        else
            printf("Busco %d y no encuentro nada\n", i);
    }
    printf("Borro todos nodos liberando la memoria:\n");
    arb = eliminararbol(arb);
    if (esarbolvacio(arb) == 0)
        printf("arbol vacío: ()\n");
    else
        printf("El arbol contiene elementos\n");
    printf("Altura del arbol: %d\n", altura(arb));
}

double CalcTiempos(int q, int tam,
                    double tiempos [3][6], int x, int j){
    double t_start=0.0; //tiempo de inicio
    double t_fin=0.0; //tiempo de fin
    double t=0.0; //tiempo del test
    int w;
    int *v;
    arbol arb;

    v = malloc(tam*sizeof(int));
    arb=creararbol();
    aleatorio(v, tam);

    t_start = microsegundos();
    for(w=0;w<tam;w++){
        arb = insertar(v[w], arb);
    }
    t_fin = microsegundos();    
                
    if(q==1){
        t_start = microsegundos();
        for(w=0;w<tam;w++){
            buscar(v[w], arb);
        }
        t_fin = microsegundos();
    }

    t = t_fin - t_start;
    tiempos[x][j] = t;
    free(v);
    arb = eliminararbol(arb);
    return t;
}

void mostrarTiempos(double tiempos[3][6]){
    int j = 0,f;
    printf("\n\nComparación de tiempos\n");
    printf("            n     t_ins(n)     t_bus(n)\n");
    for(f=0;f < 6;f++) {
        printf("%13.0f%13.2f%13.2f\n", tiempos[j][f], tiempos[j+1][f], 
                tiempos[j+2][f]);
    }
}

void obtencionTiempos(double tiempos[3][6]){
    int n, j=0;
    double t;
    
    printf("\n\nInserción de n elementos\n");
    printf("%5s%14s%13s%22s%14s\n", "n",
            "t(n)", "t/n^0.9","t/((n*log(n))^1.13)","t/n^1.6");
    for (n=8000; n <= 256000; n = n*2){
        printf("%6d",n);
        tiempos[0][j] = n;
        t= CalcTiempos(0, n, tiempos, 1, j);
        j++;
        printf("%13.2f%13.6f%22.6f%14.6f\n",t,
               t/pow(n,0.9),t/(pow(n*log(n),1.13)),t/pow(n,1.6));
    }

    j=0;

    printf("\n\nBusqueda de n elementos\n");
    printf("%5s%14s%13s%22s%14s\n", "n",
            "t(n)", "t/n^0.9","t/((n*log(n))^0.92)","t/n^1.6");
    for (n=8000; n <= 256000; n = n*2){
        printf("%6d",n);
        t=CalcTiempos(1, n, tiempos, 2, j);
        j++;
        printf("%13.2f%13.6f%22.6f%14.6f\n",t,
                t/pow(n,0.9),n/(pow(n*log(n),0.92)),n/pow(n,1.6));
    }
}

void ejecucion(){
    double tiempos[3][6];
    obtencionTiempos(tiempos);
    mostrarTiempos(tiempos);
}

int main() {
    int i;
    inicializar_semilla();
    prueba();
    printf("\n");
    for(i=0;i<1;i++){
        ejecucion();    
    }
    return 0;
}
