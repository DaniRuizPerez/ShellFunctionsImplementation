/*Práctica de SO - Curso 2012/2013 de Segundo de GEI, FIC, UDC
Grupo 2.1 (viernes 8:30)
Autores: 
	- Ismael Barbeito Vázquez (i.barbeito@udc.es)
	- Daniel Ruiz Pérez (d.ruiz.perez@udc.es)

*/

#define MAXTAM 100
#include "datommap.h"
typedef smmap datoListaMmap;

typedef struct {
	datoListaMmap datos[MAXTAM];
	int ultima;
} listammap;

void crearListaMmap(listammap* l);
int listaVaciaMmap(listammap l);
int buscarListaMmap(datoListaMmap d, listammap l,int (*comparar) (datoListaMmap a, datoListaMmap b));
datoListaMmap obtenerListaMmap (int pos, listammap l);
int eliminarListaMmap(datoListaMmap x, listammap*l,int (*comparar) (datoListaMmap a, datoListaMmap b));
int insertarListaMmap(datoListaMmap d,listammap* l);
int primeraListaMmap (listammap l);
int ultimaListaMmap (listammap l);
void vaciarListaMmap(listammap *l);
void actualizarDatoMmap(int pos, datoListaMmap a,listammap*l);
