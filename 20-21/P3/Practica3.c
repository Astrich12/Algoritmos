/*AUTORES:
	Samuel Torres González
	Julian Blanco Sóñora
	Brais Vázquez Villa
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

void aleatorio(int v [], int n) { /* se generan números pseudoaleatorio entre -n y +n */
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


int alturaArbol (arbol a){
    int AltIzq, AltDer;
    if(a == NULL)
        return -1;
    else{
        AltIzq = alturaArbol(hijoizquierdo(a));
        AltDer = alturaArbol(hijoderecho(a));
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
    arbol arb;
    arb = creararbol();
    if (esarbolvacio(arb) == 0)
        printf("El arbol está vacío\n");
    else
        printf("El arbol contiene elementos\n");
    printf("altura del arbol: %d\n",alturaArbol(arb));
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
    printf("Altura del arbol: %d\n", alturaArbol(arb));

    for(i=1; i<=6;i++){
        pos = buscar(i, arb);
        if(pos!=NULL)
            printf("Busco %d y encuentro %d repetido: %d veces\n",i, i, numerorepeticiones(pos));
        else
            printf("Busco %d y no encuentro nada\n", i);
    }
    printf("Borro todos nodos liberando la memoria:\n");

    arb = eliminararbol(arb);

    if (esarbolvacio(arb) == 0)
        printf("arbol vacío: ()\n");
    else
        printf("El arbol contiene elementos\n");

    printf("Altura del arbol: %d\n", alturaArbol(arb));
}

void mostrarTiempos(double tiempos[3][6]){
    int j = 0,f;

	printf("            n     t_ins(n)     t_bus(n)\n");
    for(f=0;f < 6;f++) {
        printf("%13.0f%13.2f%13.2f\n", tiempos[j][f], tiempos[j+1][f], tiempos[j+2][f]);
    }
}

void mostrarComplejidad(arbol(*funcion)(int e, arbol arb), int k){
    if (funcion == buscar){
        printf("Inserción de n elementos\n");
		printf("# -> tiempo promedio de %d iteraciones \n",k);
		printf("\n");
        printf("%5s%14s%13s%22s%14s\n", "n",
                "t(n)", "t/i^0.95","t/((i*log(i))^1.11)","t/i^1.6");
    }else{
        printf("Busqueda de n elementos\n");
        printf("\n");
        printf("%5s%14s%13s%22s%14s\n", "n",
                "t(n)", "t/i^0.95","t/((i*log(i))^1.13)","t/i^1.6");
    }
}

void tiempoInsertar(double x, double y, double z, double tiempos[3][6]){
    arbol arb;
    int i, v[256000], f, j = 0;
    double ta, tb, t;
	mostrarComplejidad(insertar, 0);

    for (i=8000; i <= 256000; i = i*2){
        printf("%6d",i);
        arb = creararbol();
        aleatorio(v, i);
        ta = microsegundos();
        for (f = 0; f < i; f++){
            arb = insertar(v[f], arb);
        }
        tb = microsegundos();
        t = tb - ta;

        printf("%13.2f%13.6f%22.6f%15.6f\n",t,
               t/pow(i,x),t/(pow(i*log(i), y)),t/pow(i,z));

        tiempos[0][j] = i;
        tiempos[1][j] = t;
        j++;
        arb = eliminararbol(arb);
    }
}

void tiempoBuscar(double x, double y, double z, double tiempos[3][6]){
    arbol arb;
    int i, v[256000], j, f, k = 1000, p = 0;
    double ta, tb, time1, time2, t;
	mostrarComplejidad(buscar, k);

    for (i=8000; i <= 256000; i = i*2){
        printf("%6d",i);
        arb = creararbol();
        aleatorio(v, i);
        for(j=0;j<i;j++){
            arb = insertar(v[j], arb);
        }
        ta = microsegundos();
        for (f = 0; f < i; f++){
            buscar(v[f], arb);
        }
        tb = microsegundos();
        t = tb - ta;
        if (t < 500){
            ta = microsegundos();
            for (j = 1; j <= k; j++){
                aleatorio(v, i);
                for (f = 0; f < i; f++){
                    buscar(v[f], arb);
                }
            }
            tb = microsegundos();
            time1 = tb - ta;
            ta = microsegundos();
            for (j = 1; j <= k; j++){
                aleatorio(v, i);
            }
            tb = microsegundos();
            time2 = tb - ta;
            t = (time1 - time2) / k;

            printf("%13.2f#%12.6f%22.6f%15.6f\n",t,
                   t/pow(i,x),t/(pow(i*log(i), y)),t/pow(i,z));
            tiempos[2][p] = t;p++;
            arb = eliminararbol(arb);
        }else{
            printf("%13.2f%13.6f%22.6f%15.6f\n",t,
                   t/pow(i,x),t/(pow(i*log(i), y)),t/pow(i,z));
            tiempos[2][p] = t;p++;
            arb = eliminararbol(arb);
        }
    }
}


void ejecucion(){
    double tiempos[3][6];

    tiempoInsertar(0.95, 1.13, 1.6, tiempos);
    tiempoBuscar(0.95, 1.11, 1.6, tiempos);
    printf("Comparación de tiempos\n");
    mostrarTiempos(tiempos);
}

int main() {
    inicializar_semilla();
    prueba();
	printf("\n");
    ejecucion();
    return 0;
}