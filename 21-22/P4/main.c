/*
	Julián Blanco Sóñora
	Xian Vázquez Luna
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include "colas.h"

double microsegundos() {
/* obtiene la hora del sistema en microsegundos */
	struct timeval t;
	if (gettimeofday(&t, NULL) < 0 )
		return 0.0;
	return (t.tv_usec + t.tv_sec * 1000000.0);
}

void crear_cola(cola *c){
    (*c).cabeza=0;
    (*c).final=0;
    (*c).tamano=0;

}

int cola_vacia(cola c){
    return c.tamano;
}

void insertar(tipo_elemento elem, cola *c){
    
    (*c).tamano++;
    (*c).vector[c->final]=elem;
    (*c).final++;
}

tipo_elemento quitar_primero(cola *c){
    int aux = c->cabeza;
    c->cabeza++;
    c->tamano--;
    return c->vector[aux++];
}

tipo_elemento primero(cola c){
    return c.vector[c.cabeza];
}

void mostrar_cola(cola c){  
    int peso=0;
    printf("Aristas:   ");
    while(cola_vacia(c)!=0){
        peso += primero(c).peso;
        printf("(%d,%d)  ", primero(c).x, primero(c).y);
        quitar_primero(&c);
    }
    printf("\nPeso : %d\n", peso);
}

Matriz crear_matriz(int n) {

    int i;
    Matriz aux;
    if ((aux = malloc(n*sizeof(int *))) == NULL)
        return NULL;
    for (i=0; i<n; i++)
        if ((aux[i] = malloc(n*sizeof(int))) == NULL)
            return NULL;
    return aux;
}


void inicializar_matriz(Matriz m, int n) {
/* Crea un grafo completo no dirigido con valores aleatorios entre 1 y n */
    int i, j;
    for (i=0; i<n; i++){
        for (j=i+1; j<n; j++)
            m[i][j] = rand() % n + 1;
    }
    for (i=0; i<n; i++){
        for (j=0; j<=i; j++)
            if (i==j)
                m[i][j] = 0;
            else
                m[i][j] = m[j][i];
    }
}
void liberar_matriz(Matriz m, int n) {
    int i;
    for (i=0; i<n; i++)
        free(m[i]);
    free(m);
}




void Prim(Matriz m, int nodos, cola *aristas) {
/* calcular el árbol de recubrimiento mínimo devolviendo
las aristas del arbol en la cola ’aristas’ */
    int min, i, j, k=0;

    int *masProximo = (int *) malloc(nodos*sizeof(int));
    int *distanciaMinima = (int *) malloc(nodos*sizeof(int));
    tipo_elemento *el = malloc(sizeof(tipo_elemento));
    crear_cola(aristas);
    distanciaMinima[0] = -1;
    for(i = 1; i < nodos; i++) {
        masProximo[i] = 0;
        distanciaMinima[i] = m[i][0];
    }

    for(i=0;i<nodos-1;i++){

        min = TAM_MAX;

        for(j=1;j<nodos;j++){
			if((0<=distanciaMinima[j])&& (distanciaMinima[j]<min)){
				min = distanciaMinima[j];
				k=j;         
            }
        }
        el->x = masProximo[k];
        el->peso = min;
        el->y = k;
        insertar(*el, aristas); 
    
        
        distanciaMinima[k] = -1;
        
		for(j=1;j<nodos;j++){
			if(m[j][k]<distanciaMinima[j]){
				distanciaMinima[j]=m[j][k];
				masProximo[j]=k;
			}
		}
	}
	free(masProximo);
	free(distanciaMinima);
	free(el);
}

double CalcTiempos(int tam, int k){
	double t_start=0.0; //tiempo de inicio
	double t_fin=0.0; //tiempo de fin
	double t=0.0; //tiempo del test
	int i;

	cola *a = malloc (sizeof(cola));
	Matriz m = crear_matriz(tam);
	inicializar_matriz(m, tam);  
	t_start = microsegundos();
	Prim(m, tam, a);
	t_fin = microsegundos();
	t = t_fin - t_start;

	if(t<500){
		printf("(*)");
		t_start = microsegundos();
		for(i=0;i<k;i++){
			Prim(m, tam, a);
		}
		t_fin = microsegundos();
		t = (t_fin - t_start)/k;
	}
	else printf("   ");
	liberar_matriz(m, tam);
	free(a);
	return t;
}


void test(){
/*
	int m[5][5] = {	{ 0, 1, 8, 4, 7},
			{ 1, 0, 2, 6, 5},
			{ 8, 2, 0, 9, 5},
			{ 4, 6, 9, 0, 3},
			{ 7, 5, 5, 3, 0}};
    
	int m[4][4] = { { 0, 1, 4, 7},
			{ 1, 0, 2, 8},
			{ 4, 2, 0, 3},
			{ 7, 8, 3, 0}};  

   
	int m[7][7] = {	{ 0, 7, 99, 5, 99, 99, 99},
			{ 7, 0, 8, 9, 7, 99, 99},
			{ 99, 8, 0, 99, 5, 99, 99},
			{ 5, 9, 99, 0, 15, 6, 99},
			{ 99, 7, 5, 15, 0, 8, 9},
			{ 99, 99, 99, 6, 8, 0, 11},
			{ 99, 99, 99, 99, 9, 11, 0}};
*/
	cola *a = malloc (sizeof(cola));
	Matriz m = crear_matriz(10);
	inicializar_matriz(m, 10);  

	Prim(m, 7, a);
	mostrar_cola(*a);
	free(a);
	liberar_matriz(m,10);
}

void PrintTiempos(){
	int n,k=1000;
	double tiempo; 

	printf("ALGORITMO DE PRIM:\n");
	printf("\t       n\t\t  t(n)\t\t   t(n)/n^1.80\t\t "
	       "  t(n)/n^2.09\t\t   t(n)/n^2.20\n");

	for(n=8;n<=1024;n*=2){
		tiempo = CalcTiempos(n, k);
		printf("\t% 8d\t% 14.5f\t\t% 14.8f\t\t% 14.8f\t\t% 14.8f\n", n, tiempo,
				tiempo / pow(n, 1.8), tiempo /pow(n, 2.09),
				tiempo / pow(n, 2.2));
	}

}


int main() {
	//test();
	for(int i=0;i<3;i++)
		PrintTiempos();
	return 0;
}