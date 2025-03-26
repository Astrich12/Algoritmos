/*AUTORES:
	Samuel Torres González
	Julián Blanco Sóñora
	Brais Vázquez Villa
*/

#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define intercambiar(v,i,j){ int aux = v[j]; v[j] = v[i]; v[i] = aux;}
#define UMBRAL 100

void inicializar_semilla()
{
    srand(time(NULL));
}

/* obtiene la hora actual en microsegundos */
double microsegundos()
{
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void aleatorio(int v[], int n)
{
    /* se generan numeros pseudoaleatorios entre -n y +n */
    int i, m = 2 * n + 1;
    for (i = 0; i < n; i++)
        v[i] = (rand() % m) - n;
}

void ascendente(int v[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        v[i] = i;
}

void descendente(int v[], int n)
{
    int i;
    for (i = n; i >= 0; i--)
        v[i] = n - (i + 1);
}

void rapida_aux(int v[], int izq, int der)
{
    int pivote, i, j, x;
    if (izq + UMBRAL <= der)
    {
        x = izq + rand() % (der - izq + 1);
        intercambiar(v, izq, x);
        pivote = v[x];
        i = izq + 1;
        j = der;
        while (i <= j)
        {
            while ((v[i] < pivote) & (i <= der))
                i++;

            while (v[j] > pivote)
                j--;
            
            if(i <= j){
                intercambiar(v, i, j);
                i++;
                j--;
            }
        }
        intercambiar(v, izq, j);
        rapida_aux(v, izq, j - 1);
        rapida_aux(v, j + 1, der);
    }
}

void ord_ins(int v[], int n)
{
    int i, j, x;

    for (i = 1; i < n; i++)
    {
        x = v[i];
        j = i - 1;
        while (j >= 0 && v[j] > x)
        {
            v[j + 1] = v[j];
            ;
            j--;
        }
        v[j + 1] = x;
    }
}

void ord_rapida(int v[], int n)
{
    rapida_aux(v, 0, n - 1);
    if (UMBRAL > 1)
        ord_ins(v, n);
}

void mostrar(int v[], int n)
{
    int j;
    printf("[");
    for (j = 0; j < n; j++)
    {
        printf("%d ", v[j]);
    }
    printf("]");
}

bool esOrdenado(int v[], int numposiciones)
{
    int i = 1;
    int x = 0;
    bool aux = true;
    while (i < numposiciones && aux)
    {
        if (v[i] >= v[x])
        {
            i++;
            x++;
        }
        else
        {
            aux = false;
        }
    }
    return aux;
}

void mostrarComplejidad(void(*funcion)(int v[], int n), int k, double umb){
    if (funcion == descendente){
        printf("ORDENACION RAPIDA CON INICIALIZACION DESCENDENTE\n");
            printf("\n");
            printf("# -> tiempo promedio de %d iteraciones \n",k);
            printf("%25s%21s%20s%24s%3.2f%20s\n", "n",
                    "t(n)", "t(n)/n**0.9","t(n)/(n*log(n))**", umb, "t(n)/n**2");
    }else{
        if (funcion == ascendente){
            printf("ORDENACION RAPIDA CON INICIALIZACION ASCENDENTE\n");
            printf("\n");
            printf("# -> tiempo promedio de %d iteraciones \n",k);
            printf("%25s%21s%20s%24s%3.2f%20s\n", "n",
             		"t(n)", "t(n)/n**0.9","t(n)/(n*log(n))**", umb, "t(n)/n**2");
        }else{
            printf("ORDENACION RAPIDA CON INICIALIZACION ALEATORIA\n");
            printf("\n");
            printf("# -> tiempo promedio de %d iteraciones \n",k);
            printf("%25s%21s%20s%24s%3.2f%20s\n", "n",
             		"t(n)", "t(n)/n**0.9","t(n)/(n*log(n))**", umb, "t(n)/n**2");
        }
    }
}

void tiempoMedicion(void(*funcion)(int v[], int n))
{
    int j, i = 32000, k = 1000;
    int v[i];
    double t1, t2, x, y, tiempo, umb = 1.13;
    
    if((UMBRAL == 100) & (funcion == aleatorio)){
        umb = 1;
    }else{
        if(funcion == aleatorio){
            umb = 0.95;
        }
    }

    mostrarComplejidad(funcion, k, umb);

    for (i = 500; i <= 32001; i *= 2)
    {
        funcion(v, i);
        t1 = microsegundos();
        ord_rapida(v, i);
        t2 = microsegundos();
        tiempo = t2 - t1;
        if (tiempo < 500)
        {
            t1 = microsegundos();
            for (j = 0; j < k; j++)
            {
                funcion(v, i);
                ord_rapida(v, i);
            }
            t2 = microsegundos();
            x = (t2 - t1);
            t1 = microsegundos();
            for (j = 0; j < k; j++)
            {
                funcion(v, i);
            }
            t2 = microsegundos();
            y = (t2 - t1);
            tiempo = (x - y) / k;
            printf("%25d%20.6f%s%20.6f%28.9f%20.9f\n", i, tiempo,
                   "#", tiempo / pow(i, 0.9), tiempo /pow((i * log(i)),umb),
                   tiempo / pow(i, 2));
        }
        else
        {
            printf("%25d%20.6f%21.6f%28.9f%20.9f\n", i,
                   tiempo, tiempo / pow(i, 0.9), tiempo / pow((i * log(i)),umb),
                   tiempo / pow(i, 2));
        }
    }
}

void testRapida(void(*funcion)(int v[], int n))
{
    int v[5];
    funcion(v, 5);
    mostrar(v, 5);
    printf("\n ordenado?");
    if (esOrdenado(v, 5))
    {
        printf("1");
    }
    else
    {
        printf("0");
    }
    printf("\nordenando...\n");
    ord_rapida(v, 5);
    mostrar(v, 5);
    printf("\n ordenado?");
    if (esOrdenado(v, 5))
    {
        printf("1");
    }
    else
    {
        printf("0");
    }
}

void testFinal()
{
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

void tiemposRapida()
{
    printf("\n");
    printf("____________________________________________________");
    printf("____________________________________________________");
    printf("\n");
    printf("____________________________________________________");
    printf("____________________________________________________");
    printf("\n \n");
    printf("UMBRAL = %d \n", UMBRAL);
    printf("\n");
    tiempoMedicion(ascendente);
    printf("\n");
    printf("____________________________________________________");
    printf("____________________________________________________");
    printf("\n");
    tiempoMedicion(descendente);
    printf("\n");
    printf("____________________________________________________");
    printf("____________________________________________________");
    printf("\n");
    tiempoMedicion(aleatorio);
    printf("\n");
}

int main()
{
    inicializar_semilla();
    testFinal();
    tiemposRapida();
    return 0;
}