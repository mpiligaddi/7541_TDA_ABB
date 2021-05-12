CC= gcc
WFLAGS= -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0

compilar:
	$(CC) abb.c abb_pruebas_extras.c -o abb -g $(FLAGS) 

exe: compilar 
	./abb

exe_valgrind: compilar
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./abb

compilar_gdb:
	$(CC) -g abb.c abb_pruebas_extras.c  $(FLAGS) -o abb.exe

gdb: compilar_gdb
	gdb abb.exe

compilar_minipruebas:
	$(CC) -g abb.c abb_minipruebas.c  $(FLAGS) -o abb_mini

exe_minipruebas: compilar_minipruebas
	./abb_mini

compilar_gdb_minipruebas: 
	$(CC) -g abb.c abb_minipruebas.c  $(FLAGS) -o minipruebas.exe

gdb_mini: compilar_gdb_minipruebas
	gdb minipruebas.exe

exe_mini_valgrind: compilar_minipruebas
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./abb_mini