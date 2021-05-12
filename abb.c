#include "abb.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define ERROR -1
#define EXITO 0
#define VACIO 0

// -----------------------------------------------------------------------------------
// -----------------------------FUNCIONES PRIVADAS------------------------------------
// -----------------------------------------------------------------------------------

/*
Pre: Recibe un elemento de tipo void*
Post: Crea un nuevo nodo y devuelve el mismo
*/
nodo_abb_t* arbol_nodo_crear(void* elemento) {

	nodo_abb_t* nodo_nuevo = malloc(sizeof(nodo_abb_t));
	if(!nodo_nuevo) {
		return NULL;
	}
	nodo_nuevo->elemento = elemento;
	nodo_nuevo->derecha = NULL;
	nodo_nuevo->izquierda = NULL;
	return nodo_nuevo;

}

/*
Pre: Recibe un nodo de un determinado arbol y un destructor para el elemento del mismo
Post: Libera recursivamente la memoria reservada para los nodos 
*/
void arbol_nodo_destruir(nodo_abb_t* nodo_arbol, abb_liberar_elemento destructor) {

	if(nodo_arbol->izquierda) {
		arbol_nodo_destruir(nodo_arbol->izquierda, destructor);
	}
	if(nodo_arbol->derecha) {
		arbol_nodo_destruir(nodo_arbol->derecha, destructor);
	}	

	if((destructor != NULL) && (nodo_arbol->elemento)) {
		destructor(nodo_arbol->elemento);
	}
	free(nodo_arbol);
	return;

}

/*
Pre: Funcion que inserta recursivamente los nodos a un ABB
Post: Devuelve 0 si pudo insertar el elemento que recibe o -1 si no pudo
*/
int arbol_insertar_aux(abb_t* arbol, nodo_abb_t* nodo, void* elemento) {

	if(((int*)nodo->elemento != 0) && (!nodo->elemento)) {
		return ERROR;
	}

	int comparador = arbol->comparador(nodo->elemento, elemento);

	if(comparador >= 0) {
		if(nodo->izquierda == NULL) {
			nodo_abb_t* nodo_nuevo = arbol_nodo_crear(elemento);
			nodo->izquierda = nodo_nuevo;
			return EXITO; 
		}
		return arbol_insertar_aux(arbol, nodo->izquierda, elemento);
	}
	if(comparador < 0) {
		if(nodo->derecha == NULL) {
			nodo_abb_t* nodo_nuevo = arbol_nodo_crear(elemento);
			nodo->derecha = nodo_nuevo;
			return EXITO; 
		}
		return arbol_insertar_aux(arbol, nodo->derecha, elemento);
	}
	return ERROR; 
	
}

/*
Pre: Recibe un nodo a un arbol no vacio.
Post: Devuelve el mayor nodo de los menores (predecesor inorden)
*/
nodo_abb_t* nodo_mayor_de_los_menores(nodo_abb_t* nodo) {

	while(nodo->derecha) {;
		nodo = nodo->derecha;
	}
	return nodo;

}

/*
Pre: Funcion que borra recursivamente los nodos de un ABB
Post: Devuelve el nodo que elimino 
*/
nodo_abb_t* nodo_borrar_aux(abb_t* arbol, nodo_abb_t* nodo_actual, void* elemento, bool* encontrado, int* eliminados) {

	if(!nodo_actual) {
		*encontrado = false;
		return NULL;
	}

	int comparador = arbol->comparador(nodo_actual->elemento, elemento);

	if(comparador == 0) {
		if((!nodo_actual->derecha) && (!nodo_actual->izquierda)) {
			if((arbol->destructor) && (nodo_actual->elemento)) {
				arbol->destructor(nodo_actual->elemento);
			}
			free(nodo_actual);
			return NULL;
		}
		else if((nodo_actual->derecha) && (nodo_actual->izquierda)) {
			nodo_abb_t* nodo_reemplazo = nodo_mayor_de_los_menores(nodo_actual->izquierda);
			void* elemento_aux = nodo_actual->elemento;
			nodo_actual->elemento = nodo_reemplazo->elemento;
			nodo_reemplazo->elemento = elemento_aux;
			(*eliminados)++;
			nodo_actual->izquierda = nodo_borrar_aux(arbol,nodo_actual->izquierda,elemento,encontrado,eliminados);
		}
		else {
			nodo_abb_t* nodo_a_devolver = nodo_actual->derecha ? nodo_actual->derecha : nodo_actual->izquierda;
			if((arbol->destructor) && (nodo_actual->elemento)) {
				arbol->destructor(nodo_actual->elemento);
			}
			free(nodo_actual);
			return nodo_a_devolver;
		}
	}
	else if(comparador > 0) {
		nodo_actual->izquierda = nodo_borrar_aux(arbol,nodo_actual->izquierda,elemento,encontrado,eliminados);
	}
	else if(comparador < 0) {
		nodo_actual->derecha = nodo_borrar_aux(arbol,nodo_actual->derecha,elemento,encontrado,eliminados);
	}
	return nodo_actual;

}

/*
Pre: Recibe un arbol no NULL, un nodo al mismo y un elemento para insertar en el arbol
Post: Devuelve el elemento que se buscaba o NULL si no esta en el arbol
*/
void* arbol_buscar_aux(abb_t* arbol, nodo_abb_t* nodo, void* elemento) {

	if(!nodo) {
		return NULL;
	}

	int comparador = arbol->comparador(nodo->elemento, elemento);
	
	if(comparador == 0) {
		return nodo->elemento;
	}
	if(comparador > 0) {
		return arbol_buscar_aux(arbol, nodo->izquierda, elemento);
	}
	if(comparador < 0) {
		return arbol_buscar_aux(arbol, nodo->derecha, elemento);
	}
	return NULL;

}

/*
Pre: Recibe un ABB, un nodo a ese ABB, un array para guardar los elementos de los nodos recibidos por parametro, un tamanio_array que es el tope del array y un contador. Recorre el arbol con un recorrido inorden
Post: Devuelve la cantidad de elementos que fueron almacenados en el array
*/
size_t arbol_recorrido_inorden_aux(abb_t* arbol,nodo_abb_t* nodo,void** array,size_t tamanio_array, size_t* contador){

	if((nodo) && ((*contador) < tamanio_array)) {
		arbol_recorrido_inorden_aux(arbol, nodo->izquierda, array, tamanio_array, contador); 
		if(((*contador) < tamanio_array) && (nodo->elemento)) {
			array[*contador] = nodo->elemento;
			(*contador)++;
		}
		arbol_recorrido_inorden_aux(arbol, nodo->derecha, array, tamanio_array, contador);
	}
	return (*contador);

}

/*
Pre: Recibe un ABB, un nodo a ese ABB, un array para guardar los elementos de los nodos recibidos por parametro, un tamanio_array que es el tope del array y un contador. Recorre el arbol con un recorrido preorden
Post: Devuelve la cantidad de elementos que fueron almacenados en el array
*/
size_t arbol_recorrido_preorden_aux(abb_t* arbol,nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* contador) {

	if((nodo) && ((*contador) < tamanio_array)) {
		if(((*contador) < tamanio_array) && (nodo->elemento)) {
			array[*contador] = nodo->elemento;
			(*contador)++;
		}
		arbol_recorrido_preorden_aux(arbol, nodo->izquierda, array, tamanio_array, contador);
		arbol_recorrido_preorden_aux(arbol, nodo->derecha, array, tamanio_array, contador);
	}
	return (*contador);

}

/*
Pre: Recibe un ABB, un nodo a ese ABB, un array para guardar los elementos de los nodos recibidos por parametro, un tamanio_array que es el tope del array y un contador. Recorre el arbol con un recorrido postorden
Post: Devuelve la cantidad de elementos que fueron almacenados en el array
*/
size_t arbol_recorrido_postorden_aux(abb_t* arbol,nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* contador) {

	if((nodo) && ((*contador) < tamanio_array)) {
		arbol_recorrido_postorden_aux(arbol, nodo->izquierda, array, tamanio_array, contador);
		arbol_recorrido_postorden_aux(arbol, nodo->derecha, array, tamanio_array, contador);
		if(((*contador) < tamanio_array) && (nodo->elemento)) {
			array[*contador] = nodo->elemento;
			(*contador)++;
		}
	}
	return (*contador);

}

/*
Pre: Recorre el arbol con un recorrido inorden
Post: Devuelve la cantidad de elementos recorridos
*/
size_t arbol_recorrer_inorden(nodo_abb_t* nodo,bool (*funcion)(void*, void*),void* extra, size_t* contador, bool* encontrado){

	if((nodo) && (*encontrado == false)) {
		arbol_recorrer_inorden(nodo->izquierda, funcion, extra, contador, encontrado);
		if(*encontrado == false) {
			if(funcion(nodo->elemento, extra) == true) {
				(*encontrado) = true;
				return (*contador);
			}
		}
		(*contador)++;
		arbol_recorrer_inorden(nodo->derecha, funcion, extra, contador, encontrado);
	}
	return (*contador);

}

/*
Pre: Recorre el arbol con un recorrido preorden
Post: Devuelve la cantidad de elementos recorridos
*/
size_t arbol_recorrer_preorden(nodo_abb_t* nodo,bool (*funcion)(void*, void*),void* extra, size_t* contador,bool* encontrado){

	if((!nodo) || (*encontrado == true)) {
		return (*contador);
	}
	if((nodo) && (*encontrado == false)) { 
		if(*encontrado == false) {
			if(funcion(nodo->elemento, extra) == true) {
				(*contador)++;
				(*encontrado) = true;
				return (*contador);
			}
		}
		(*contador)++;
		arbol_recorrer_preorden(nodo->izquierda, funcion, extra, contador,encontrado);
		arbol_recorrer_preorden(nodo->derecha, funcion, extra, contador,encontrado);
		return (*contador);
	}	
	return (*contador);
}

/*
Pre: Recorre el arbol con un recorrido postorden
Post: Devuelve la cantidad de elementos recorridos
*/

size_t arbol_recorrer_postorden(nodo_abb_t* nodo,bool (*funcion)(void*, void*),void* extra, size_t* contador, bool* encontrado){

	if((nodo) && (*encontrado == false)) {
		arbol_recorrer_postorden(nodo->izquierda, funcion, extra, contador, encontrado);
		arbol_recorrer_postorden(nodo->derecha, funcion, extra, contador,encontrado);
		if(*encontrado == false) {
			if(funcion(nodo->elemento, extra) == true) {
				(*encontrado) = true;
				return (*contador);
			}
		}
		(*contador)++;
	}
	return (*contador);

}

// -----------------------------------------------------------------------------------
// -----------------------------FUNCIONES PUBLICAS------------------------------------
// -----------------------------------------------------------------------------------

abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor) {

	if(!comparador) {
		return NULL;
	}
	abb_t* arbol_nuevo = NULL;
	arbol_nuevo = malloc(sizeof(abb_t));
	if(!arbol_nuevo) {
		return NULL;
	}
	arbol_nuevo->nodo_raiz = NULL;
	arbol_nuevo->comparador = comparador;
	arbol_nuevo->destructor = destructor;
	return arbol_nuevo;

}

int arbol_insertar(abb_t* arbol, void* elemento) {

	if(!arbol) {
		return ERROR;
	}
	if(arbol_vacio(arbol) == true) {
		nodo_abb_t* nodo_nuevo = arbol_nodo_crear(elemento);
		if(!nodo_nuevo) {
			return ERROR;
		}
		arbol->nodo_raiz = nodo_nuevo;
		return EXITO;
	}
	return arbol_insertar_aux(arbol, arbol->nodo_raiz, elemento);

}

int arbol_borrar(abb_t* arbol, void* elemento) {

	bool encontrado = true;
	int eliminados = 0;

	if((arbol_vacio(arbol) == true) || (arbol->comparador == NULL) || (arbol_buscar(arbol,elemento) == NULL)) {
		return ERROR;
	}
	arbol->nodo_raiz = nodo_borrar_aux(arbol, arbol->nodo_raiz, elemento, &encontrado, &eliminados);
	if((encontrado == false) && (eliminados = 0)) {
		return ERROR;
	}
	return EXITO;
}

void* arbol_buscar(abb_t* arbol, void* elemento) {

	if((arbol_vacio(arbol) == true) || (arbol->comparador == NULL) || (elemento == NULL)) {
		return NULL;
	}
	return arbol_buscar_aux(arbol, arbol->nodo_raiz, elemento);

}


void* arbol_raiz(abb_t* arbol) {

	if(arbol_vacio(arbol) == true) {
		return NULL;
	}
	return (arbol->nodo_raiz->elemento);

}

bool arbol_vacio(abb_t* arbol) {

	if((!arbol) || (!arbol->nodo_raiz)) {
		return true;
	}
	return false;

}

size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array) {

	if((arbol_vacio(arbol) == true) || (!array)) {
		return VACIO;
	}
	size_t contador = 0;
	return arbol_recorrido_inorden_aux(arbol, arbol->nodo_raiz, array, tamanio_array, &contador);

}

size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array) {

	if((arbol_vacio(arbol) == true) || (!array)) {
		return VACIO;
	}
	size_t contador = 0;
	return arbol_recorrido_preorden_aux(arbol, arbol->nodo_raiz, array, tamanio_array, &contador);

}

size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array) {

	if((arbol_vacio(arbol) == true) || (!array)) {
		return VACIO;
	}
	size_t contador = 0;
	return arbol_recorrido_postorden_aux(arbol, arbol->nodo_raiz, array, tamanio_array, &contador);

}

void arbol_destruir(abb_t* arbol) {

	if(arbol_vacio(arbol) == true) {
		free(arbol);
		return;
	}
	if(arbol->nodo_raiz) {
		arbol_nodo_destruir(arbol->nodo_raiz,arbol->destructor);
	}
	free(arbol);

}

size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra) {

	size_t contador = 0;
	bool encontrado = false;

	if((arbol_vacio(arbol) == true) || (!funcion)) {
		return VACIO;
	}

	if(recorrido == ABB_RECORRER_INORDEN) {
		return arbol_recorrer_inorden(arbol->nodo_raiz, funcion, extra, &contador, &encontrado);
	}

	else if(recorrido == ABB_RECORRER_PREORDEN) {
		return arbol_recorrer_preorden(arbol->nodo_raiz, funcion, extra, &contador, &encontrado);
	}

	else if(recorrido == ABB_RECORRER_POSTORDEN) {
		return arbol_recorrer_postorden(arbol->nodo_raiz, funcion, extra, &contador, &encontrado);
	}

	return VACIO;

}