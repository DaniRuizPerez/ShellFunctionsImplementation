/*Práctica de SO - Curso 2012/2013 de Segundo de GEI, FIC, UDC
Grupo 2.1 (viernes 8:30)
Autores: 
	- Ismael Barbeito Vázquez (i.barbeito@udc.es)
	- Daniel Ruiz Pérez (d.ruiz.perez@udc.es)

*/

#define MAXRUTA 4096
#include <sys/resource.h>
typedef struct {
	int pid;
	char comando[MAXRUTA];
	int time;
	char estado[20];
	int causa;
	struct rusage ru;
}sproc;

