/*
	Julián Blanco Sóñora
	Xian Vázquez Luna
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <sys/time.h>

#define intercambiar(v,i,j){ int aux = v[j]; v[j] = v[i]; v[i] = aux;}
#define UMBRAL 100

double microsegundos() {
/* obtiene la hora del sistema en microsegundos */
	struct timeval t;
	if (gettimeofday(&t, NULL) < 0 )
		return 0.0;
	return (t.tv_usec + t.tv_sec * 1000000.0);
}

void inicializar_semilla() {
	srand(time(NULL));
}
void mediana(int v[], int n){
	int k =0+n/2;
	if(v[k]>v[n]) intercambiar(v, k,n);
	if(v[k]>v[0]) intercambiar(v,0,k);
	if(v[0]>v[n]) intercambiar(v, 0,n);
}
void Ordenar_aux(int v[], int izq, int der){
	int pivote, i, j;
	if (izq + UMBRAL <= der){
		mediana(v, der);
		pivote = v[izq];
		i=izq;
		j=der;
		while (j <= i){
			for(i=i+1;v[i]>=pivote;i++);
			for(j=j-1;v[j]<=pivote;j--);
			intercambiar(v, i, j);
			}
		intercambiar(v, i, j);
		intercambiar(v, izq, j);
		Ordenar_aux(v, izq, j - 1);
		Ordenar_aux(v, j + 1, der);
	}
}

void ord_ins(int v[], int n){
	int i,x,j;
	for(i=1;i<n;i++){
		x=v[i];
		j=i-1;
		while(j>=0 && v[j]>x){
			v[j+1]=v[j];
			j=j-1;
		}
		v[j+1]=x;
	}
}

void ordenacionRapida(int v[], int n){
	Ordenar_aux(v,0,n);
	if(UMBRAL>1)
		ord_ins(v, n);
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
	for (i = n; i >= 0; i--)
		v[i] = n - (i + 1);
}

void listar_vector(int v[],int n){
	int i;
	printf("[ ");
	for (i = 0;i<n;i++){
		printf("%4d",v[i]);
	}
	printf(" ] \n");
}

bool esOrdenado(int v[], int numposiciones){
	int i = 1;
	int x = 0;
	bool aux = true;
	while (i < numposiciones && aux){
		if (v[i] >= v[x]){
			i++;
			x++;
		}else
			aux = false;
	}
	return aux;
}

void testInsercion(void(*funcion)(int v[], int n)){
	int v[5];
	funcion(v, 5);
	listar_vector(v, 5);
	printf("\n ordenado?");

	if (esOrdenado(v, 5)) printf("1");
	else printf("0");

	printf("\nordenando...\n");
	ord_ins(v, 5);
	listar_vector(v, 5);
	printf("\n ordenado? ");

	if (esOrdenado(v, 5)) printf("1\n");
	else printf("0\n");
}

void testRapida(void(*funcion)(int v[], int n)){
	int v[5];
	funcion(v, 5);
	listar_vector(v, 5);
	printf("\n ordenado?");

	if (esOrdenado(v, 5)) printf("1");
	else printf("0");

	printf("\nordenando...\n");
	ordenacionRapida(v, 5);
	listar_vector(v, 5);
	printf("\n ordenado? ");

	if (esOrdenado(v, 5)) printf("1\n");
	else printf("0\n");
}

void testFinal(){
	printf("TEST ORDENACION INSERCION\n");
	printf("Ordenacion por inserción con");
	printf(" inicializacion aleatoria\n");
	testInsercion(aleatorio);
	printf("Ordenacion por insercion con");
	printf(" inicializacion descendente\n");
	testInsercion(descendente);
	printf("Ordenacion por inserción con");
	printf(" inicializacion ascendente\n");
	testInsercion(ascendente);

	printf("TEST ORDENACION RAPIDA\n");
	printf("Ordenacion por ordenacion rapida con");
	printf(" inicializacion aleatoria\n");
	testRapida(aleatorio);
	printf("Ordenacion por ordenacion rapida con");
	printf(" inicializacion descendente\n");
	testRapida(descendente);
	printf("Ordenacion por ordenacion rapida con");
	printf(" inicializacion ascendente\n");
	testRapida(ascendente);
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


void print_ins(){
	int n,k=1000;
	double tiempo;

	printf("ORDENACION POR INSERCION\n");

	printf("VECTORES INICIADOS ASCENDENTEMENTE:\n");
	printf("\t      n\t\t\t  t(n)\t\t\t    t(n)/n^0.85\t\t\t  "
			"       t(n)/n\t\t\t    t(n)/n^1.15\n");
	for(n=500;n<=32000;n*=2){
		tiempo = CalcTiempos(ord_ins, n, ascendente, k);
		printf("\t% 8d\t% 14.5f\t\t\t% 14.12f\t\t\t% 14.12f\t\t\t% 14.12f\n", n,
				tiempo, tiempo/pow(n,0.85), tiempo/n,
				tiempo/(pow(n,1.15)));
	}
	printf("\n");

	printf("VECTORES INICIADOS DESCENDENTEMENTE:\n");
	printf("\t      n\t\t\t  t(n)\t\t\t    t(n)/n^1.80\t\t\t  "
			"  t(n)/n^2.00\t\t\t    t(n)/n^2.20\n");
	for(n=500;n<=32000;n*=2){
		tiempo = CalcTiempos(ord_ins, n, descendente, k);
		printf("\t% 8d\t% 14.5f\t\t\t% 14.12f\t\t\t% 14.12f\t\t\t% 14.12f\n", n,
				tiempo, tiempo/(pow(n,1.8)), tiempo/pow(n,2),
				tiempo/(pow(n,2.2)));
	}
	printf("\n");

	printf("VECTORES INICIADOS ALEATORIAMENTE:\n");
	printf("\t      n\t\t\t  t(n)\t\t\t    t(n)/n^1.80\t\t\t  "
			"  t(n)/n^2.00\t\t\t    t(n)/n^2.20\n");
	for(n=500;n<=32000;n*=2){
		tiempo = CalcTiempos(ord_ins, n, aleatorio, k);
		printf("\t% 8d\t% 14.5f\t\t\t% 14.12f\t\t\t% 14.12f\t\t\t% 14.12f\n", n,
				tiempo, tiempo/(pow(n,1.8)), tiempo/pow(n,2),
				tiempo/(pow(n,2.2)));
	}
	printf("\n");

}

void print_aux(){
	int n,k=1000;
	double umb = 0.90; 
	double tiempo;

	printf("VECTORES INICIADOS ASCENDENTEMENTE:\n");
	printf("\t      n\t\t\t  t(n)\t\t   t(n)/(n*log(n))^0.75\t\t "
	       "  t(n)/(n*log(n))^0.90\t\t   t(n)/(n*log(n))^1.05\n");
	for(n=500;n<=32000;n*=2){
		tiempo = CalcTiempos(ordenacionRapida, n, ascendente, k);
		printf("\t% 8d\t% 14.5f\t\t\t% 14.12f\t\t\t% 14.12f\t\t\t% 14.12f\n", n, tiempo,
				tiempo / pow(n * log(n),0.75), tiempo /pow(n * log(n),umb),
				tiempo / pow(n * log(n),1.05));
	}
	printf("\n");

	printf("VECTORES INICIADOS DESCENDENTEMENTE:\n");
	printf("\t      n\t\t\t  t(n)\t\t   t(n)/(n*log(n))^0.75\t\t "
	       "  t(n)/(n*log(n))^0.90\t\t   t(n)/(n*log(n))^1.05\n");
	for(n=500;n<=32000;n*=2){
		tiempo = CalcTiempos(ordenacionRapida, n, descendente, k);
		printf("\t% 8d\t% 14.5f\t\t\t% 14.12f\t\t\t% 14.12f\t\t\t% 14.12f\n", n, tiempo,
				tiempo / pow(n * log(n),0.75), tiempo /pow(n * log(n),umb),
				tiempo / pow(n * log(n),1.05));
	}
    printf("\n");

	printf("VECTORES INICIADOS ALEATORIAMENTE:\n");
	printf("\t      n\t\t\t  t(n)\t\t   t(n)/(n*log(n))^0.75\t\t "
	       "  t(n)/(n*log(n))^0.90\t\t   t(n)/(n*log(n))^1.05\n");

	for(n=500;n<=32000;n*=2){
		tiempo = CalcTiempos(ordenacionRapida, n, aleatorio, k);
		printf("\t% 8d\t% 14.5f\t\t\t% 14.12f\t\t\t% 14.12f\t\t\t% 14.12f\n", n, tiempo,
				tiempo / pow(n * log(n),0.75), tiempo /pow(n * log(n),umb),
				tiempo / pow(n * log(n),1.05));
	}
	printf("\n");
}

void print_sel(){
	int n,k=1000; 
	double umb = 0.9;
	double tiempo;

	printf("UMBRAL = %d \n", UMBRAL);
	printf("ORDENACION POR SELECCION\n\n");

 	if(UMBRAL==1){
 		print_aux();
 		return;
 	}

 	if(UMBRAL==100) umb = 0.94;

	printf("VECTORES INICIADOS ASCENDENTEMENTE:\n");
	printf("\t      n\t\t\t  t(n)\t\t   t(n)/(n*log(n))^0.75\t\t "
	       "  t(n)/(n*log(n))^0.90\t\t   t(n)/(n*log(n))^1.05\n");
	for(n=500;n<=32000;n*=2){
		tiempo = CalcTiempos(ordenacionRapida, n, ascendente, k);
		printf("\t% 8d\t% 14.5f\t\t\t% 14.12f\t\t\t% 14.12f\t\t\t% 14.12f\n", n, tiempo,
				tiempo / pow(n * log(n),0.75), tiempo /pow(n * log(n),umb),
				tiempo / pow(n * log(n),1.05));
	}
	printf("\n");

	printf("VECTORES INICIADOS DESCENDENTEMENTE:\n");
	printf("\t      n\t\t\t  t(n)\t\t\t    t(n)/n^1.80\t\t\t  "
			"  t(n)/n^2.00\t\t\t    t(n)/n^2.20\n");
	for(n=500;n<=32000;n*=2){
		tiempo = CalcTiempos(ordenacionRapida, n, descendente, k);
		printf("\t% 8d\t% 14.5f\t\t\t% 14.12f\t\t\t% 14.12f\t\t\t% 14.12f\n", n, tiempo,
				tiempo / pow(n, 1.80), tiempo /pow(n,2),
				tiempo / pow(n, 2.20));
	}
    printf("\n");

	printf("VECTORES INICIADOS ALEATORIAMENTE:\n");
	printf("\t      n\t\t\t  t(n)\t\t\t    t(n)/n^1.80\t\t\t  "
			"  t(n)/n^2.00\t\t\t    t(n)/n^2.20\n");

	for(n=500;n<=32000;n*=2){
		tiempo = CalcTiempos(ordenacionRapida, n, aleatorio, k);
		printf("\t% 8d\t% 14.5f\t\t\t% 14.12f\t\t\t% 14.12f\t\t\t% 14.12f\n", n, tiempo,
				tiempo / pow(n, 1.80), tiempo /pow(n,2),
				tiempo / pow(n, 2.2));
	}
	printf("\n");
}

int main() {
	int i;
	inicializar_semilla();
	testFinal();
	for(i=0;i<3;i++){
		print_ins();
		print_sel();
	}

	return 0;
}