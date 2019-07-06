/*Práctica de SO - Curso 2012/2013 de Segundo de GEI, FIC, UDC
Grupo 2.1 (viernes 8:30)
Autores: 
	- Ismael Barbeito Vázquez (i.barbeito@udc.es)
	- Daniel Ruiz Pérez (d.ruiz.perez@udc.es)

*/

#define MAXTAM 100
#include "datomalloc.h"
typedef smalloc datoListaMalloc;

typedef struct {
	datoListaMalloc datos[MAXTAM];
	int ultima;
} listamalloc;

void crearListaMalloc(listamalloc* l);
int listaVaciaMalloc(listamalloc l);
int buscarListaMalloc(datoListaMalloc d, listamalloc l,int (*comparar) (datoListaMalloc a, datoListaMalloc b));
datoListaMalloc obtenerListaMalloc (int pos, listamalloc l);
int eliminarListaMalloc(datoListaMalloc x, listamalloc*l,int (*comparar) (datoListaMalloc a, datoListaMalloc b));
int insertarListaMalloc(datoListaMalloc d,listamalloc* l);
int primeraListaMalloc (listamalloc l);
int ultimaListaMalloc (listamalloc l);
void vaciarListaMalloc(listamalloc *l);
void actualizarDato(int pos, datoListaMalloc a,listamalloc*l);
