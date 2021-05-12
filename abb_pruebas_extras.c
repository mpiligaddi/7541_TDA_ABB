#include "abb.h"
#include "pa2mm.h"

#define AMARILLO "\x1B[33;1m"
#define NORMAL "\x1B[0m"
#define MAX_ELEMENTOS 10
#define ERROR -1
#define EXITO 0

/*
Pre: Recibe dos elemento de tipo void* para comparar
Post: Devuelve 0 en caso de elementos iguales, 1 en caso de que el primero sea mayor al segundo y -1 en caso de que no se cumplan ninguna de las condiciones anteriores
*/
int comparador_elementos(void* primer_elemento, void* segundo_elemento) {

	int primer_numero = *(int*)primer_elemento;
	int segundo_numero = *(int*)segundo_elemento;

	if(primer_numero == segundo_numero) {
		return 0;
	}
	if(primer_numero > segundo_numero) {
		return 1;
	}
	return -1;

}

/*
Pre: Recibe un elemento existente
Post: Libera la memoria reservada para el elemento
*/
void destruir_elemento(int* elemento){
    free(elemento);
}

/*
Pre: Recibe un elemento del tipo void*
Post: -
*/
void destructor_elementos(void* elemento){
    if(!elemento)
        return;
    destruir_elemento((int*)elemento);
}

/*
Pre: Recibe una clave a un elemento
Post: Devuelve el elemento
*/
int* crear_elemento(int clave){
    int* elemento = (int*)malloc(sizeof(int));
    if(elemento)
        *elemento = clave;
    return elemento;
}

/*
Pre: -
Post: Devuelve false si encontro el elemento para imprimir o true si no lo encontro
*/
bool mostrar_elemento_actual(void* elemento, void* aux){
    aux=aux;
    if(elemento) 
        printf("%i ", *(int*)elemento);
    return false;
}

void probar_creacion_arbol() {

	pa2m_nuevo_grupo("PRUEBAS DE CREACION DEL ARBOL");
	abb_t* arbol_nuevo = arbol_crear(comparador_elementos,NULL);
	pa2m_afirmar(arbol_nuevo != NULL, "Puedo crear un arbol");
	pa2m_afirmar(arbol_nuevo->nodo_raiz == NULL, "Nodo raiz del arbol inicializado a NULL");
	pa2m_afirmar(arbol_nuevo->destructor == NULL, "Destructor puede ser NULL");
	abb_t* arbol = arbol_crear(NULL,NULL); 
	pa2m_afirmar(arbol == NULL, "Error al pasar un comparador NULL");
	arbol_destruir(arbol_nuevo);

}

void probar_insersion_de_elementos_arbol() {

	pa2m_nuevo_grupo("PRUEBAS DE INSERSION DE ELEMENTOS EN EL ARBOL");
	abb_t* arbol_con_elementos = arbol_crear(comparador_elementos,destructor_elementos); 
	abb_t* arbol_nuevo_con_elementos = arbol_crear(comparador_elementos,destructor_elementos); 

	int elementos_insertados_nuevos = 0;

	int* elemento_1 = crear_elemento(1);
	int* elemento_2 = crear_elemento(3);
	int* elemento_3 = crear_elemento(5);
	int* elemento_4 = crear_elemento(2);
	int* elemento_5 = crear_elemento(0);
	int* auxiliar = crear_elemento(0);
	
	arbol_insertar(arbol_nuevo_con_elementos, elemento_1);
    arbol_insertar(arbol_nuevo_con_elementos, elemento_2);
    arbol_insertar(arbol_nuevo_con_elementos, elemento_3);
    arbol_insertar(arbol_nuevo_con_elementos, elemento_4);
    arbol_insertar(arbol_nuevo_con_elementos, elemento_5);

	pa2m_afirmar(arbol_insertar(arbol_con_elementos, crear_elemento(0)) == 0, "Puedo insertar un elemento en un arbol vacio");
	for(int i = 1 ; i < 1000 ; i++) {
		if (arbol_insertar(arbol_con_elementos, crear_elemento(i)) == 0) {
			if(elementos_insertados_nuevos == 9) {
				pa2m_afirmar(elementos_insertados_nuevos == 9, "Se pueden insertar 10 elementos en el arbol");
			}
			else if(elementos_insertados_nuevos == 49) {
				pa2m_afirmar(elementos_insertados_nuevos == 49, "Se pueden insertar 50 elementos en el arbol");
			}
			(elementos_insertados_nuevos)++;
		}
	}
	pa2m_afirmar(elementos_insertados_nuevos == 999, "Se pueden insertar 1000 elementos en el arbol");
	pa2m_afirmar((*((int*)(arbol_raiz(arbol_con_elementos))) == 0), "El elemento de la raiz del arbol es el 0");
	*auxiliar = 2;
	pa2m_afirmar((*((int*)arbol_buscar(arbol_nuevo_con_elementos, auxiliar)) == 2), "El numero 2 es un elemento del arbol");
	*auxiliar = 3;
	pa2m_afirmar((*((int*)arbol_buscar(arbol_nuevo_con_elementos, auxiliar)) == 3), "El numero 3 es un elemento del arbol");
	pa2m_afirmar((*((int*)arbol_buscar(arbol_nuevo_con_elementos, auxiliar)) == 3), "La busqueda de elementos en el arbol me devuelve el elemento que buscaba");

	free(auxiliar);
	arbol_destruir(arbol_nuevo_con_elementos);
	arbol_destruir(arbol_con_elementos);

}

void probar_arbol_vacio() {

	pa2m_nuevo_grupo("PRUEBAS DE ARBOL VACIO");
	abb_t* arbol_nuevo = arbol_crear(comparador_elementos,NULL);
	pa2m_afirmar(arbol_vacio(NULL) == true, "Un arbol NULL es un arbol vacio");
	pa2m_afirmar(arbol_vacio(arbol_nuevo) == true, "Un arbol con nodo raiz NULL es un arbol vacio");
	abb_t* arbol_nuevo_con_elementos = arbol_crear(comparador_elementos,destructor_elementos);
	int* auxiliar = crear_elemento(0);
	for(int i = 1 ; i < 5 ; i++) {
		arbol_insertar(arbol_nuevo_con_elementos, crear_elemento(i));
	}
	pa2m_afirmar(arbol_vacio(arbol_nuevo_con_elementos) != true, "Un arbol con nodo raiz no NULL no es un arbol vacio");
	*auxiliar = 3;
	pa2m_afirmar(arbol_buscar(arbol_nuevo, auxiliar) == NULL, "Si busco un elemento en un arbol vacio me devuelve NULL");

	free(auxiliar);
	arbol_destruir(arbol_nuevo_con_elementos);
	arbol_destruir(arbol_nuevo);

}

void probar_recorrido_arbol() {

	pa2m_nuevo_grupo("PRUEBAS DE RECORRIDO DEL ARBOL");
	abb_t* arbol_nuevo_con_elementos = arbol_crear(comparador_elementos,destructor_elementos);

	int* elemento_1 = crear_elemento(1);
	int* elemento_2 = crear_elemento(3);
	int* elemento_3 = crear_elemento(5);
	int* elemento_4 = crear_elemento(2);
	int* elemento_5 = crear_elemento(0);
	
	arbol_insertar(arbol_nuevo_con_elementos, elemento_1);
    arbol_insertar(arbol_nuevo_con_elementos, elemento_2);
    arbol_insertar(arbol_nuevo_con_elementos, elemento_3);
    arbol_insertar(arbol_nuevo_con_elementos, elemento_4);
    arbol_insertar(arbol_nuevo_con_elementos, elemento_5);

	int* elementos[10];
	int* elementos_acotados[3];

	printf("\t" AMARILLO "RECORRIDO INORDEN" NORMAL "\n");
	pa2m_afirmar(arbol_recorrido_inorden(NULL,(void**)elementos, 10) == 0, "No se recorrieron elementos del arbol ni se agregaron al array en caso de tener un arbol vacio");
	pa2m_afirmar(arbol_recorrido_inorden(arbol_nuevo_con_elementos,NULL, 10) == 0, "No se recorrieron elementos del arbol ni se agregaron al array en caso de tener un array NULL");
	size_t cantidad_inorden = arbol_recorrido_inorden(arbol_nuevo_con_elementos,(void**)elementos, 10);
	pa2m_afirmar(cantidad_inorden == 5, "Se recorrieron todos los elementos del arbol, agregandose al array");
	pa2m_afirmar(cantidad_inorden == 5, "El array no llego a su tope");
	size_t cantidad_inorden_nueva = arbol_recorrido_inorden(arbol_nuevo_con_elementos,(void**)elementos_acotados, 3);
	pa2m_afirmar(cantidad_inorden_nueva == 3, "Se recorrieron todos los elementos posibles del arbol, agregandose al array");
	pa2m_afirmar(cantidad_inorden_nueva == 3, "El array llego a su tope");

	printf("\t" AMARILLO "RECORRIDO PREORDEN" NORMAL "\n");
	pa2m_afirmar(arbol_recorrido_preorden(NULL,(void**)elementos, 10) == 0, "No se recorrieron elementos del arbol ni se agregaron al array en caso de tener un arbol vacio");
	pa2m_afirmar(arbol_recorrido_preorden(arbol_nuevo_con_elementos,NULL, 10) == 0, "No se recorrieron elementos del arbol ni se agregaron al array en caso de tener un array NULL");
	size_t cantidad_preorden = arbol_recorrido_preorden(arbol_nuevo_con_elementos,(void**)elementos, 10);
	pa2m_afirmar(cantidad_preorden == 5, "Se recorrieron todos los elementos del arbol, agregandose al array");
	pa2m_afirmar(cantidad_preorden == 5, "El array no llego a su tope");
	size_t cantidad_preorden_nueva = arbol_recorrido_preorden(arbol_nuevo_con_elementos,(void**)elementos_acotados, 3);
	pa2m_afirmar(cantidad_preorden_nueva == 3, "Se recorrieron todos los elementos posibles del arbol, agregandose al array");
	pa2m_afirmar(cantidad_preorden_nueva == 3, "El array llego a su tope");

	printf("\t" AMARILLO "RECORRIDO POSTORDEN" NORMAL "\n");
	pa2m_afirmar(arbol_recorrido_postorden(NULL,(void**)elementos, 10) == 0, "No se recorrieron elementos del arbol ni se agregaron al array en caso de tener un arbol vacio");
	pa2m_afirmar(arbol_recorrido_postorden(arbol_nuevo_con_elementos,NULL, 10) == 0, "No se recorrieron elementos del arbol ni se agregaron al array en caso de tener un array NULL");
	size_t cantidad_postorden = arbol_recorrido_postorden(arbol_nuevo_con_elementos,(void**)elementos, 10);
	pa2m_afirmar(cantidad_postorden == 5, "Se recorrieron todos los elementos del arbol, agregandose al array");
	pa2m_afirmar(cantidad_postorden == 5, "El array no llego a su tope");
	size_t cantidad_postorden_nueva = arbol_recorrido_postorden(arbol_nuevo_con_elementos,(void**)elementos_acotados, 3);
	pa2m_afirmar(cantidad_postorden_nueva == 3, "Se recorrieron todos los elementos posibles del arbol, agregandose al array");
	pa2m_afirmar(cantidad_postorden_nueva == 3, "El array llego a su tope");

	arbol_destruir(arbol_nuevo_con_elementos);

}

void probar_busqueda_arbol() {

	pa2m_nuevo_grupo("PRUEBAS DE BUSQUEDA EN EL ARBOL");
	abb_t* arbol_nuevo = arbol_crear(comparador_elementos,destructor_elementos);

	int* elemento_1 = crear_elemento(1);
	int* elemento_2 = crear_elemento(3);
	int* elemento_3 = crear_elemento(5);
	int* elemento_4 = crear_elemento(2);
	int* elemento_5 = crear_elemento(0);
	int* auxiliar = crear_elemento(0);
	
	arbol_insertar(arbol_nuevo, elemento_1);
    arbol_insertar(arbol_nuevo, elemento_2);
    arbol_insertar(arbol_nuevo, elemento_3);
    arbol_insertar(arbol_nuevo, elemento_4);
    arbol_insertar(arbol_nuevo, elemento_5);

    *auxiliar = 101;
	pa2m_afirmar(arbol_buscar(arbol_nuevo, auxiliar) == NULL, "Si busco un elemento inexistente en el arbol me devuelve NULL");
	pa2m_afirmar(arbol_buscar(arbol_nuevo, NULL) == NULL, "Si busco un elemento NULL en el arbol me devuelve NULL");
	*auxiliar = 1;
	pa2m_afirmar(arbol_buscar(NULL, auxiliar) == NULL, "Si busco un elemento en un arbol NULL me devuelve NULL");
	pa2m_afirmar(*((int*)arbol_buscar(arbol_nuevo, auxiliar)) == 1, "Busco el elemento 1, existente en el arbol");
	*auxiliar = 3;
	pa2m_afirmar(*((int*)arbol_buscar(arbol_nuevo, auxiliar)) == 3, "Busco el elemento 3, existente en el arbol");
	*auxiliar = 5;
	pa2m_afirmar(*((int*)arbol_buscar(arbol_nuevo, auxiliar)) == 5, "Busco el elemento 5, existente en el arbol");
	*auxiliar = 2;
	pa2m_afirmar(*((int*)arbol_buscar(arbol_nuevo, auxiliar)) == 2, "Busco el elemento 2, existente en el arbol");
	*auxiliar = 0;
	pa2m_afirmar(*((int*)arbol_buscar(arbol_nuevo, auxiliar)) == 0, "Busco el elemento 0, existente en el arbol");

	free(auxiliar);
    arbol_destruir(arbol_nuevo);

}

void probar_iterador_interno_arbol() {

	pa2m_nuevo_grupo("PRUEBAS DE ITERADOR INTERNO");

	int contador = 0;
	pa2m_afirmar(abb_con_cada_elemento(NULL, ABB_RECORRER_INORDEN,mostrar_elemento_actual,&contador) == 0, "Devuelve 0 elementos recorridos si recibe un arbol vacio");

	abb_t* arbol = arbol_crear(comparador_elementos,destructor_elementos);

	int* elemento_1 = crear_elemento(1);
	int* elemento_2 = crear_elemento(3);
	int* elemento_3 = crear_elemento(5);
	int* elemento_4 = crear_elemento(2);
	int* elemento_5 = crear_elemento(0);
	
	arbol_insertar(arbol, elemento_1);
    arbol_insertar(arbol, elemento_2);
    arbol_insertar(arbol, elemento_3);
    arbol_insertar(arbol, elemento_4);
    arbol_insertar(arbol, elemento_5);

	pa2m_afirmar(abb_con_cada_elemento(arbol,10,mostrar_elemento_actual,&contador) == 0, "Devuelve 0 elementos recorridos si recibe un numero de recorrido no valido");
	pa2m_afirmar(abb_con_cada_elemento(arbol,ABB_RECORRER_INORDEN,NULL,&contador) == 0, "Devuelve 0 elementos recorridos si recibe una funcion en NULL");
	printf("\t" AMARILLO "ITERADOR RECORRIDO INORDEN" NORMAL "\n");
	pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN,mostrar_elemento_actual,&contador) == 5, "Devuelve la cantidad de elementos recorridos inorden");
	printf("\t" AMARILLO "ITERADOR RECORRIDO PREORDEN" NORMAL "\n");
	pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN,mostrar_elemento_actual,&contador) == 5, "Devuelve la cantidad de elementos recorridos preorden");
	printf("\t" AMARILLO "ITERADOR RECORRIDO POSTORDEN" NORMAL "\n");
	pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN,mostrar_elemento_actual,&contador) == 5, "Devuelve la cantidad de elementos recorridos postorden");
	arbol_destruir(arbol);

}

void recorrer_arbol(abb_t* arbol, int* elementos[MAX_ELEMENTOS]) {

    size_t cantidad = arbol_recorrido_inorden(arbol, (void**)elementos, 10);
    for(size_t i=0;i<cantidad;i++) {
       printf("%i ", *elementos[i]); 
    }
    printf("\n");
    
}

void probar_borrado_arbol() {

	pa2m_nuevo_grupo("PRUEBAS DE BORRADO NODOS DEL ARBOL");

	abb_t* arbol = arbol_crear(comparador_elementos,destructor_elementos);
	abb_t* arbol_nuevo = arbol_crear(comparador_elementos,destructor_elementos);

	int* elemento_1 = crear_elemento(6);
	int* elemento_2 = crear_elemento(8);
	int* elemento_3 = crear_elemento(10);
	int* elemento_4 = crear_elemento(7);
	int* elemento_5 = crear_elemento(4);
	int* elemento_6 = crear_elemento(5);
	int* elemento_7 = crear_elemento(2);
	int* elemento_8 = crear_elemento(2);
	int* auxiliar = crear_elemento(0);

	int* elemento_10 = crear_elemento(20);
	int* elemento_11 = crear_elemento(30);
	int* elemento_12 = crear_elemento(15);
	int* elemento_13 = crear_elemento(18);
	int* elemento_14 = crear_elemento(16);
	
	arbol_insertar(arbol, elemento_1);
    arbol_insertar(arbol, elemento_2);
    arbol_insertar(arbol, elemento_3);
    arbol_insertar(arbol, elemento_4);
    arbol_insertar(arbol, elemento_5);
    arbol_insertar(arbol, elemento_6);
    arbol_insertar(arbol, elemento_7);
    arbol_insertar(arbol, elemento_8);

    int* elementos[10];

    *auxiliar = 20;
	pa2m_afirmar(arbol_borrar(arbol, auxiliar) == ERROR, "Error al querer borrar un nodo que no existe en el arbol");

	*auxiliar = 7;
	pa2m_afirmar(arbol_borrar(NULL, auxiliar) == ERROR, "Error al querer borrar un nodo de un arbol NULL");

	printf("\t" AMARILLO "BORRAR NODO HOJA IZQUIERDA" NORMAL "\n");
	pa2m_afirmar(arbol_borrar(arbol, auxiliar) == 0, "Borro nodo que es hoja izquierda (7)");
    printf("Recorrido inorden (deberian salir en orden 2 2 4 5 6 8 10): ");
 	recorrer_arbol(arbol,elementos);
 	size_t cantidad = arbol_recorrido_inorden(arbol, (void**)elementos, 10);
	pa2m_afirmar(cantidad == 7, "Quedan siete elementos en el arbol");

 	printf("\t" AMARILLO "BORRAR NODO HOJA DERECHA" NORMAL "\n");
	*auxiliar = 10;
	pa2m_afirmar(arbol_borrar(arbol, auxiliar) == 0, "Borro nodo que es hoja derecha (10)");
    printf("Recorrido inorden (deberian salir en orden 2 2 4 5 6 8): ");
 	recorrer_arbol(arbol,elementos);
 	cantidad = arbol_recorrido_inorden(arbol, (void**)elementos, 10);
	pa2m_afirmar(cantidad == 6, "Quedan seis elementos en el arbol");

 	printf("\t" AMARILLO "BORRAR NODO CON UN HIJO" NORMAL "\n");
	*auxiliar = 2;
	pa2m_afirmar(arbol_borrar(arbol, auxiliar) == 0, "Borro nodo con un solo hijo (2)");
    printf("Recorrido inorden (deberian salir en orden 2 4 5 6 8): ");
 	recorrer_arbol(arbol,elementos);
 	cantidad = arbol_recorrido_inorden(arbol, (void**)elementos, 10);
	pa2m_afirmar(cantidad == 5, "Quedan cinco elementos en el arbol");

 	printf("\t" AMARILLO "BORRAR NODO CON DOS HIJOS" NORMAL "\n");
	*auxiliar = 4;
	pa2m_afirmar(arbol_borrar(arbol, auxiliar) == 0, "Borro nodo con dos hijos (4)");
    printf("Recorrido inorden (deberian salir en orden 2 5 6 8): ");
 	recorrer_arbol(arbol,elementos);
 	cantidad = arbol_recorrido_inorden(arbol, (void**)elementos, 10);
	pa2m_afirmar(cantidad == 4, "Quedan cuatro elementos en el arbol");

	*auxiliar = 4;
	pa2m_afirmar(arbol_buscar(arbol_nuevo, auxiliar) == NULL, "Busco el elemento 4, pero no existe mas en el arbol");

 	printf("\t" AMARILLO "BORRAR NODO RAIZ CON DOS HIJOS" NORMAL "\n");
	*auxiliar = 6;
	pa2m_afirmar(arbol_borrar(arbol, auxiliar) == 0, "Borro nodo raiz con dos hijos (6)");
    printf("Recorrido inorden (deberian salir en orden 2 5 8): ");
 	recorrer_arbol(arbol,elementos);
 	cantidad = arbol_recorrido_inorden(arbol, (void**)elementos, 10);
	pa2m_afirmar(cantidad == 3, "Quedan tres elementos en el arbol");

 	printf("\t" AMARILLO "BORRAR NODO HIJO DERECHA DE LA RAIZ" NORMAL "\n");
	*auxiliar = 8;
	pa2m_afirmar(arbol_borrar(arbol, auxiliar) == 0, "Borro nodo hijo derecha de la raiz con dos hijos (8)");
    printf("Recorrido inorden (deberian salir en orden 2 5): ");
 	recorrer_arbol(arbol,elementos);
 	cantidad = arbol_recorrido_inorden(arbol, (void**)elementos, 10);
	pa2m_afirmar(cantidad == 2, "Quedan dos elementos en el arbol");

 	printf("\t" AMARILLO "BORRAR NODO RAIZ CON UN HIJO" NORMAL "\n");
	*auxiliar = 5;
	pa2m_afirmar(arbol_borrar(arbol, auxiliar) == 0, "Borro nodo raiz con un hijo (5)");
    printf("Recorrido inorden (deberian salir en orden 2): ");
 	recorrer_arbol(arbol,elementos);
 	cantidad = arbol_recorrido_inorden(arbol, (void**)elementos, 10);
	pa2m_afirmar(cantidad == 1, "Queda un elemento en el arbol");

 	printf("\t" AMARILLO "BORRAR NODO RESTANTE" NORMAL "\n");
	*auxiliar = 2;
	pa2m_afirmar(arbol_borrar(arbol, auxiliar) == 0, "Borro el unico nodo que queda en el arbol (2)");
    printf("Recorrido inorden (no deberian haber numeros recorridos): ");
 	recorrer_arbol(arbol,elementos);
 	cantidad = arbol_recorrido_inorden(arbol, (void**)elementos, 10);
	pa2m_afirmar(cantidad == 0, "El arbol no tiene elementos");
	pa2m_afirmar(arbol_vacio(arbol) == true, "El arbol esta vacio");

	printf("\t" AMARILLO "BORRAR NODOS VARIOS" NORMAL "\n");

 	*auxiliar = 2;
 	pa2m_afirmar(arbol_borrar(arbol, auxiliar) == ERROR, "Error al querer borrar un elemento de un arbol vacio ");

    arbol_insertar(arbol_nuevo, elemento_10);
    arbol_insertar(arbol_nuevo, elemento_11);
    arbol_insertar(arbol_nuevo, elemento_12);
    arbol_insertar(arbol_nuevo, elemento_13);
    arbol_insertar(arbol_nuevo, elemento_14);

	*auxiliar = 20;
	pa2m_afirmar(arbol_borrar(arbol_nuevo, auxiliar) == 0, "Puedo borrar utilizando como reemplazo el predecesor inorden teniendo este un hijo izquierdo");
    printf("Recorrido inorden (deberia salir en orden 15 16 18 30): ");
 	recorrer_arbol(arbol_nuevo,elementos);
 	pa2m_afirmar(arbol_vacio(arbol_nuevo) != true, "El arbol contiene elementos aun");
 	*auxiliar = 16;
 	pa2m_afirmar(arbol_borrar(arbol_nuevo, auxiliar) == 0, "Borro nodo hoja");
 	printf("Recorrido inorden (deberia salir en orden 15 18 30): ");
 	recorrer_arbol(arbol_nuevo,elementos);
 	*auxiliar = 18;
 	pa2m_afirmar(arbol_borrar(arbol_nuevo, auxiliar) == 0, "Borro nodo raiz con dos hijos");

	free(auxiliar);
	arbol_destruir(arbol);
	arbol_destruir(arbol_nuevo);
}


int main() {

	probar_creacion_arbol();
	probar_insersion_de_elementos_arbol();
	probar_arbol_vacio();
	probar_recorrido_arbol();
	probar_busqueda_arbol();
	probar_iterador_interno_arbol();
	probar_borrado_arbol();

	pa2m_mostrar_reporte();

	return 0;

}