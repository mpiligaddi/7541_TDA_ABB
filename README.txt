README: TDA ABB.

Facultad de Ingeniería - UBA.

Cátedra Mendez - Camejo.

Alumna: Maria Pilar Gaddi.

Primera sección: Presentación del TDA.

	-Introducción: 
		El presente TDA consta de cinco archivos para poder ser compilado y ejecutado correctamente:

			abb.c: donde se implementan todas las funciones de la biblioteca abb.h brindada por la cátedra (funciones públicas). También allí se encuentran las funciones privadas. 

			abb.h: donde se encuentran las firmas de las funciones públicas a implementar.

			abb_minipruebas.c: donde se encuentran las "pruebas guia" del TDA, con todos los "casos felices". La misma fue brindada por la cátedra y la utilice para tener un panorama general de como estaba funcionando el programa. 

			abb_pruebas_extras.c: donde se encuentran implementadas todas las funciones que tienen como objetivo ser parte de las "pruebas propias" del TDA, basandome en los anteriores "casos felices" y teniendo en cuenta los "casos borde".

			pa2mm.h: donde se encuentran algunas de las funciones utilizadas en abb_pruebas_extras.c para realizar las pruebas. Esta biblioteca también fue brindada por la cátedra. 

	-Linea de compilación: 
		El programa debe ser compilado bajo la siguiente linea de compilación:
			gcc *.c -o abb -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0

	-Linea de ejecución: 
		El programa debe ser ejecutado bajo la siguiente linea de ejecución:
			./abb
		En caso de que se desee chequear que no haya perdida de memoria al correr el programa:
			valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./abb

	ACLARACION: En caso de querer utilizar el main de abb_minipruebas.c, este se encuentra abajo de todo como comentario. Sacar de comentario y comentar el main "principal" de abb_pruebas_extras.c. La linea de compilación y ejecución son las mismas mencionadas anteriormente, al igual que el chequeo de memoria con valgrind.

Segunda sección: Preguntas teóricas.

	-Explicar qué es un ABB y cómo se diferencia de un Arbol Binario

	Un árbol binario está relacionado con las operaciones de búsqueda y,cuando se realiza una de estas, hay que tener en cuenta donde se debe seguir buscando: hacia la derecha o hacia la izquierda. Esta noción de derecha e izquierda que se permite tener se da gracias a este tipo de árboles. Sin embargo, un TDA árbol binario no tiene mucha utilidad por sí mismo ya que no tiene ninguna regla a seguir para poder insertar en él elementos. 
	Como solución, se nos presenta el Árbol Binario de Búsqueda (ABB), este es un TDA que tiene las características de un Árbol Binario, pero que además son árboles de comparación. Esto quiere decir que si uno se mueve hacia la derecha del árbol irá hacia los elementos más grandes del mismo, en cambio, si uno se mueve hacia la izquierda del árbol, irá hacia los elementos más pequeños del mismo.

	-Explicar cúal es el objetivo de tener una función de destrucción en el TDA y qué implicaría para el usuario no tenerla.

	En el TDA ABB la función destrucción permite liberar la memoria reservada para los elementos de los nodos existentes en el ABB, cuando el árbol en cuestión posee un destructor. 
	En este caso, antes de que se libere la memoria reservada por los nodos existentes en el árbol, se deberán liberar los elementos que se encuentran en cada uno de estos. Esto se debe a que ahora parte de la implementación es que el usuario pueda poner elementos del heap en el árbol y,mandando una función de destrucción, le da la responsabilidad al programador para que este se encargue de liberarlos. Si el usuario no tendría la función de destrucción a su disposición, los elementos de cada uno de los nodos del ABB no podrían ser liberados y se perdería memoria una vez finalizado el programa.

	-¿Cúal es la complejidad de las diferentes operaciones del ABB? Justifique.

	Operación arbol_crear, arbol_vacio y arbol_raiz: La complejidad es O(1) ya que se trata de una constante, es decir, no depende del tamaño del problema.

	Operación arbol_destruir: La complejidad es O(n) ya que en este caso, en el peor de los casos,se recorren todos los nodos existentes en el arbol. En caso de que el arbol este vacio, su complejidad cambia a O(1) ya que no debo recorrer ningun nodo y solo debo liberar la memoria reservada para el arbol, lo cual seria el mejor de los casos. 

	Operación arbol_insertar,arbol_borrar y arbol_buscar: La complejidad es O(log(n)) ya que en este caso el problema se va dividiendo a la mitad en cada iteración que va pasando. Esto sucede únicamente cuando el ABB esta balanceado. En caso de que no este balanceado (se degenera en lista), no se podrá dividir a la mitad el problema y por eso mismo la complejidad será O(n). Por lo tanto, en el mejor de los casos la complejidad seria O(log(n)) y en el peor de los casos O(n).

	Operación arbol_recorrer: La complejidad es O(n) ya que en este caso se van recorriendo todos los nodos del arbol hasta que se cumpla una determinada condición. En el peor de los casos se recorre todo el arbol. En el caso particular del TP, para todos los recorridos (inorden,preorden,postorden) sería el mismo caso. 
