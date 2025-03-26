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

void inicializar_semilla() {
  srand(time(NULL));
}

void aleatorio(int v [], int n) {/* se generan números pseudoaleatorio entre -n y +n */
  int i, m=2*n+1;
  for (i=0; i < n; i++)
    v[i] = (rand() % m) - n;
}

double microsegundos() {    /* obtiene la hora del sistema en microsegundos */
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

void ord_ins(int v[], int n){
  int i, x, j;
  for (i = 1;i < n;i++){
    x = v[i];
    j = i - 1;
    while ((j >= 0)&&(v[j] > x)){
      v[j+1] = v[j];
      j = j - 1;
    }
    v[j+1] = x;
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
        }else{
          seguir = false;
        }
      }
      v[j] = tmp;
    }
  }
}

void test_ordenacion(void(*ordenacion)(int v[],int n), int n){
  int v[n];
  printf("\n");

  printf("ORDENACION POR INSERCION \n");
  ordenacion(v, n);
  listar_vector(v, n);
  printf("Ordenado? 0\n");
  printf("ORDENANDO...\n");
  ord_ins(v, n);
  listar_vector(v, n);
  printf("Ordenado? 1\n");

  printf("\n");

  printf("ORDENACION SHELL\n");
  ordenacion(v, n);
  listar_vector(v, n);
  printf("Ordenado? 0\n");
  printf("ORDENANDO...\n");
  ord_shell(v, n);
  listar_vector(v, n);
  printf("Ordenado? 1\n");

}

void mostrarComplejidad(void(*funcion)(int v[],int n), void(*ordenacion)(int v[],int n),int k){
  if (funcion == ord_ins){
  	if (ordenacion == descendente){
  		printf("ORDENACION POR INSERCION CON INICIALIZACION DESCENDENTE\n");
  		printf("\n");
		printf("# -> tiempo promedio de %d iteraciones \n",k);
		printf("%5s%14s%13s%13s%14s\n", "n",
               	  "t(n)", "t(n)/n**1.8","t(n)/n**2","t(n)/n**2.2");
  	}else{
  		if (ordenacion == ascendente){
  			printf("ORDENACION POR INSERCION CON INICIALIZACION ASCENDENTE\n");
  			printf("\n");
  			printf("# -> tiempo promedio de %d iteraciones \n",k);
			printf("%5s%14s%13s%13s%14s\n", "n",
                 		"t(n)", "t(n)/n**0.8","t(n)/n**1","t(n)/n**1.12");
  		}else{
  			printf("ORDENACION POR INSERCION CON INICIALIZACION DESORDENADA\n");
  			printf("\n");
  			printf("# -> tiempo promedio de %d iteraciones \n",k);
			printf("%5s%14s%13s%13s%14s\n", "n",
                 		"t(n)", "t(n)/n**1.8","t(n)/n**2","t(n)/n**2.2");
  		}
  	}
  }else{
    	if (ordenacion == descendente){
    		printf("ORDENACION SHELL CON INICIALIZACION DESCENDENTE\n");
    		printf("\n");
  		printf("# -> tiempo promedio de %d iteraciones \n",k);
		printf("%5s%14s%13s%13s%14s\n", "n",
                 	"t(n)", "t(n)/n**0.95","t(n)/n**1.15","t(n)/n**1.38");
  	}else{
  		if (ordenacion == ascendente){
  			printf("ORDENACION POR SHELL CON INICIALIZACION ASCENDENTE\n");
  			printf("\n");
  			printf("# -> tiempo promedio de %d iteraciones \n",k);
			printf("%5s%14s%13s%13s%14s\n", "n",
                 		"t(n)", "t(n)/n**1","t(n)/n**1.2","t(n)/n**1.4");
  		}else{
  			printf("ORDENACION POR SHELL CON INICIALIZACION DESORDENADA\n");
  			printf("\n");
  			printf("# -> tiempo promedio de %d iteraciones \n",k);
			printf("%5s%14s%13s%13s%14s\n", "n",
				"t(n)", "t(n)/n**1.15","t(n)/n**1.3","t(n)/n**1.5");
  		}
  	}
  }
}


void test_final(void(*funcion)(int v[],int n),void(*ordenacion)(int v[],int n), double x, double y, double z){
  int i, j, v[32000], k = 10;
  double ta, tb, time1, time2, t;

  mostrarComplejidad(funcion, ordenacion, k);

	for (i=500; i <= 32000; i = i*2){
		printf("%5d",i);
		ordenacion(v, i);
		ta = microsegundos();
		funcion(v,i);
		tb = microsegundos();
		t = tb - ta;
     		if (t < 500){
       		ta = microsegundos();
       		for (j = 1; j <= k; j++){
         			ordenacion(v,i);
         			funcion(v,i);
       		}
       		tb = microsegundos();
      			time1 = tb - ta;
       		ta = microsegundos();
       		for (j = 1; j <= k; j++){
        			 ordenacion(v,i);
      			}
       		tb = microsegundos();
       		time2 = tb - ta;
       		t = (time1 - time2) / k;

       		printf("%13.2f#%12.6f%13.6f%13.6f\n",t,
         		t/pow(i,x),t/pow(i,y),t/pow(i,z));
     		}else{
	     		printf("%13.2f%13.6f%13.6f%13.6f\n",t,
		     	t/pow(i,x),t/pow(i,y),t/pow(i,z));
     		}
	}
}

void ejecucion(){
  printf("--------------------------------------------------------------- \n");
  printf("TEST INICIALIZACION ASCENDENTE\n");
  test_ordenacion(ascendente,10);
  printf("--------------------------------------------------------------- \n");
  test_final(ord_ins, ascendente, 0.8, 1, 1.12);
  printf("\n");
  test_final(ord_shell, ascendente, 1, 1.2, 1.4);
  printf("--------------------------------------------------------------- \n");
  printf("TEST INICIALIZACION DESCENDENTE\n");
  test_ordenacion(descendente,10);
  printf("--------------------------------------------------------------- \n");
  test_final(ord_ins, descendente, 1.8, 2, 2.2);
  printf("\n");
  test_final(ord_shell, descendente, 0.95, 1.15, 1.38);
  printf("--------------------------------------------------------------- \n");
  printf("TEST INICIALIZACION ALEATORIA\n");
  test_ordenacion(aleatorio,17);
  printf("--------------------------------------------------------------- \n");
  test_final(ord_ins, aleatorio, 1.8, 2, 2.2);
  printf("\n");
  test_final(ord_shell, aleatorio, 1.15, 1.3, 1.5);
}

int main () {
  inicializar_semilla();
  ejecucion();

  return 0;
}
