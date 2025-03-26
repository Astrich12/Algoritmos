/*
	Julián Blanco Sóñora
	Xian Vázquez Luna
*/

#define TAM_MAX 1600

typedef int ** Matriz;

typedef struct {
	int x, y, peso;
} arista;

typedef arista tipo_elemento;

typedef struct {
	int cabeza, final, tamano;
	tipo_elemento vector[TAM_MAX];
} cola;

void crear_cola(cola *);

int cola_vacia(cola);

void insertar(tipo_elemento, cola *);

tipo_elemento quitar_primero(cola *);

tipo_elemento primero(cola);

void mostrar_cola(cola);
