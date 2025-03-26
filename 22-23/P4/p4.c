                /*PRACTICA 4*/
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

#define TAM 512000
#define INTERCAMBIAR(v,i,j){ int aux = v[j]; v[j] = v[i]; v[i] = aux;}

typedef struct {
	int vector[TAM];
	int ultimo;
}monticulo;

void listar_vector(int v[],int n){
	int i;
	printf("[ ");
	for (i = 0;i<n;i++){
		printf("%4d",v[i]);
	}
	printf(" ] \n");
}

void inicializar_semilla() {
        srand(time(NULL));
}

double microsegundos() {
/* obtiene la hora del sistema en microsegundos */
        struct timeval t;
        if (gettimeofday(&t, NULL) < 0 )
                return 0.0;
        return (t.tv_usec + t.tv_sec * 1000000.0);
}

void inicializarMonticulo(monticulo *mont){
	mont->ultimo=-1;
}

bool monticuloVacio(monticulo *mont){
	return mont->ultimo==-1;
}

void hundir(monticulo *mont, int var){
	int hijoDer, hijoIzq, j;
	do{
		hijoIzq = 2*var+1;
		hijoDer = 2*var+2;
		j = var;
		if(hijoDer<=mont->ultimo && 
			mont->vector[hijoDer] > mont->vector[var]){
			var = hijoDer;
		}
		if(hijoIzq<=mont->ultimo && 
			mont->vector[hijoIzq] > mont->vector[var]){
			var = hijoIzq;
		}
		INTERCAMBIAR(mont->vector, j, var);
	}while(j!=var);
}

void crear_monticulo(int v[], monticulo *mont, int tam){
	int i;
	for(i=0; i<tam;i++){
		mont->vector[i] = v[i];
	}
	mont->ultimo = tam-1;
	for(i=(tam-1)/2; i>=0;i--){
		hundir(mont, i);
	}
}

int eliminar_mayor(monticulo *mont){
	int x, aux;
	if(monticuloVacio(mont)){
		printf("Monticulo vacio");
	}else{
		aux = mont->ultimo;
		x = mont->vector[0];
		mont->vector[0] = mont->vector[aux];
		mont->ultimo = aux-1;

		if(mont->ultimo > -1){
			hundir(mont, 0);
		}
		return x;
	}
	return -1;
}

void aleatorio(int v[], int n){
        /* se generan numeros pseudoaleatorios entre -n y +n */
        int i, m = 2 * n + 1;
        for (i = 0; i < n; i++)
                v[i] = (rand() % m) - n;
}

void ascendente(int v[], int n){
        int i;
        for (i = 0; i < n; i++)
                v[i] = i;
}

void descendente(int v[], int n){
        int i;
        for (i = n-1; i >= 0; i--)
                v[i] = n - (i + 1);
}

void ordenacionPorMonticulos(int v[], int tam){
	monticulo *mont = malloc(sizeof(monticulo));
	int i;
	crear_monticulo(v,mont, tam);
	for(i=tam-1; i>=0;i--){
		if(tam==10) listar_vector(mont->vector, mont->ultimo+1);
		v[i] = eliminar_mayor(mont);
	}
	free(mont);

}

void test_crear(){
	printf("Test crear_monticulo\n");
	monticulo *mont = malloc(sizeof(monticulo));
	inicializarMonticulo(mont);
	int v[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	crear_monticulo(v, mont, 9);
	listar_vector(mont->vector, 9);
	free(mont);
}

void test_ordenar(){
	printf("Test ordenacionPorMonticulos\n");
	int *vector = malloc(10*sizeof(int));
	ascendente(vector, 10);
	ordenacionPorMonticulos(vector, 10);
	free(vector);
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
        else printf("   ");
        free(v);
        return t;	
}

void auxCrear(int v[], int tam){
	monticulo *mont = malloc(sizeof(monticulo));
	inicializarMonticulo(mont);
	crear_monticulo(v, mont, tam);
	free(mont);
}


void print_ordenacion(void(*orden)(int v[], int tam), 
			void (*algoritmo)(int v[], int tam)){
        int n,k=1000;
        double tiempo;
        double ajustada = 1.00;


        if(algoritmo==ordenacionPorMonticulos) ajustada = 1.065;
        if(orden==descendente) ajustada= 1.06;
        if(orden==aleatorio) ajustada = 1.11;
	printf("\t      n\t\t  t(n)\t   t(n)/n^0.80\t  "
		"t(n)/n^%.3f\t   t(n)/n^1.20\n", ajustada);
        for(n=2000;n<=TAM;n*=2){
                tiempo = CalcTiempos(algoritmo, n, orden, k);
                printf("\t% 8d% 14.5f\t% 14.6f\t% 14.6f\t"
                        "% 14.6f\n", n,tiempo, tiempo/pow(n,0.80),
                        tiempo/(pow(n,ajustada)), tiempo/(pow(n,1.20)));
        }
        printf("\n");
}

void calculoComplejidades(){
	printf("Calculo tiempos crear_monticulo\n");
	print_ordenacion(ascendente, auxCrear);
	printf("Calculo tiempos ordenacionPorMonticulos\n");
	printf("VECTORES INICIADOS ASCENDENTEMENTE:\n");
	print_ordenacion(ascendente, ordenacionPorMonticulos);
	printf("VECTORES INICIADOS DESCENDENTEMENTE:\n");
	print_ordenacion(descendente, ordenacionPorMonticulos);
	printf("VECTORES INICIADOS ALEATORIAMENTE:\n");
	print_ordenacion(aleatorio, ordenacionPorMonticulos);
}

int main(){
	int i;
	inicializar_semilla();
	test_crear();
	test_ordenar();
	for(i=0;i<3;i++){
		calculoComplejidades();
	}
	return 0;
}
