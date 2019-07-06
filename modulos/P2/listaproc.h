/*Práctica de SO - Curso 2012/2013 de Segundo de GEI, FIC, UDC
Grupo 2.1 (viernes 8:30)
Autores: 
	- Ismael Barbeito Vázquez (i.barbeito@udc.es)
	- Daniel Ruiz Pérez (d.ruiz.perez@udc.es)

*/

#define MAXTAM 100
#include "datoproc.h"
typedef sproc datoListaProc;

typedef struct {
	datoListaProc datos[MAXTAM];
	int ultima;
} listaProc;

void crearListaProc(listaProc* l);
int listaVaciaProc(listaProc l);
int buscarListaProc(datoListaProc d, listaProc l,int (*comparar) (datoListaProc a, datoListaProc b));
datoListaProc obtenerListaProc (int pos, listaProc l);
int eliminarListaProc(datoListaProc x, listaProc*l,int (*comparar) (datoListaProc a, datoListaProc b));
int insertarListaProc(datoListaProc d,listaProc* l);
int primeraListaProc (listaProc l);
int ultimaListaProc (listaProc l);
void vaciarListaProc(listaProc *l);
void actualizarDatoProc(int pos, datoListaProc a,listaProc*l);
