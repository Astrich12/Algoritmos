/*PRACTICA 1*/
/*nombre: BRAIS VAZQUEZ VILLA */
/*login: brais.vazquez.villa@udc.es*/


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

void mostrarTipo(int(*funcion)(int n)){
  if (funcion == fib1){
    printf("Test de la sucesión de fibonacci con el algoritmo fib1:\n");
  }else{
    if(funcion == fib2){
      printf("Test de la sucesión de fibonacci con el algoritmo fib2:\n");
    }else{
      printf("Test de la sucesión de fibonacci con el algoritmo fib3:\n");
    }
  }
}

void test_funcionamiento(int (*funcion)(int n)){
	int n = 10, v[n];
	ascendente(v,n);
	mostrarTipo(funcion);
	
	for(int i = 0; i < n; i++){
		printf("%d ",funcion(v[i]));
	}
	
	printf("\n");
}

void tiempo_fib1(){
	int v[5] = {2,4,8,16,32}, k = 100;
	double t1,t2,tiempo,x,y,z;
	
	printf("Comparación de los tiempos de ejecución del algoritmo fib1: \n");
	printf("%14s%15s%15s%24s%15s\n", "n", "fib1",
			"t(n)/1.1**n"," t(n)/(1+sqrt(5)/2)**n","t(n)/2**n");
	
	for (int i = 0; i<5; i++){
		t1 = microsegundos();
		fib1(v[i]);
		t2 = microsegundos();
		tiempo = t2 - t1;
		if (tiempo < 500){
			t1 = microsegundos();
			for (int j = 1; j <= k; j++){
				fib1(v[i]);
			}
			t2 = microsegundos();
			tiempo = (t2 - t1) / k;
			x = tiempo / pow(1.1,v[i]);
			y = tiempo / pow(1+sqrt(5)/2,v[i]);
			z = tiempo / pow(2, v[i]);
			printf("# %12d%15.5f%15.9f%24.9f%15.9f\n", v[i], tiempo, x, y, z);
		}else{
			x = tiempo / pow(1.1,v[i]);
			y = tiempo / pow(1+sqrt(5)/2,v[i]);
			z = tiempo / pow(2, v[i]);
			printf("%14d%15.5f%15.9f%24.9f%15.9f\n", v[i], tiempo, x, y, z);
		}
	}
	printf("\n");
	printf("#Tiempo menor a 500 microsegundos \n");
	printf("*Tiempo promedio de k = %d iteraciones \n",k);
}

void tiempos_fib2o3(int (*funcion)(int n), int k){
	int v[5] = {1000,10000,100000,1000000,10000000}; double t1,t2,tiempo,x,y,z;
	
	if (funcion == fib2){
		printf("%14s%15s%15s%15s%18s\n", "n", "fib2",
			"t(n)/n**0.8"," t(n)/n","t(n)/n**log(n)");
	}else{
		printf("%14s%15s%20s%15s%15s\n", "n", "fib3",
			"t(n)/sqrt(log(n))"," t(n)/log(n)","t(n)/n**0.5");
	}
	
	for (int i = 0; i<5; i++){
		t1 = microsegundos();
		funcion(v[i]);
		t2 = microsegundos();
		tiempo = t2 - t1;
		if (tiempo < 500){
			t1 = microsegundos();
			for (int j = 1; j <= k; j++){
				funcion(v[i]);
			}
			t2 = microsegundos();
			tiempo = (t2 - t1) / k;
			if (funcion == fib2){
				x = tiempo / pow(v[i], 0.8); y = tiempo / v[i]; z = tiempo / (v[i] * log(v[i]));
				printf("# %12d%15.5f%15.9f%15.9f%18.9f\n", v[i], tiempo, x, y, z);
			}else{
				x = tiempo / sqrt(log(v[i])); y = tiempo / log(v[i]); z = tiempo / pow(v[i], 0.5);
				printf("# %12d%15.5f%20.9f%15.9f%15.9f\n", v[i], tiempo, x, y, z);
			}
		}else{
			if (funcion == fib2){
				x = tiempo / pow(v[i], 0.8); y = tiempo / v[i]; z = tiempo / (v[i] * log(v[i]));
				printf("%14d%15.5f%15.9f%15.9f%18.9f\n", v[i], tiempo, x, y, z);
			}else{
				x = tiempo / sqrt(log(v[i])); y = tiempo / log(v[i]); z = tiempo / pow(v[i], 0.5);
				printf("%14d%15.5f%20.9f%15.9f%15.9f\n", v[i], tiempo, x, y, z);
			}
		}
	}
	printf("\n"); printf("#Tiempo menor a 500 microsegundos \n");
	printf("*Tiempo promedio de k = %d iteraciones \n",k);
}

void tiempos_ejecucion(){
	tiempo_fib1();
	printf("\n");
	printf("Comparación de los tiempos de ejecución del algoritmo fib2: \n");
	tiempos_fib2o3(fib2,1000);
	printf("\n");
	printf("Comparación de los tiempos de ejecución del algoritmo fib3: \n");
	tiempos_fib2o3(fib3,100000);
}

int main () {
	test_funcionamiento(fib1);
	test_funcionamiento(fib2);
	test_funcionamiento(fib3);
	printf("\n");
	tiempos_ejecucion();
	return 0;
}
