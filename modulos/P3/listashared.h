/*Práctica de SO - Curso 2012/2013 de Segundo de GEI, FIC, UDC
Grupo 2.1 (viernes 8:30)
Autores: 
	- Ismael Barbeito Vázquez (i.barbeito@udc.es)
	- Daniel Ruiz Pérez (d.ruiz.perez@udc.es)

*/

#define MAXTAM 100
#include "datoshared.h"
typedef sshared datoListaShared;

typedef struct {
	datoListaShared datos[MAXTAM];
	int ultima;
} listashared;

void crearListaShared(listashared* l);
int listaVaciaShared(listashared l);
int buscarListaShared(datoListaShared d, listashared l,int (*comparar) (datoListaShared a, datoListaShared b));
datoListaShared obtenerListaShared (int pos, listashared l);
int eliminarListaShared(datoListaShared x, listashared*l,int (*comparar) (datoListaShared a, datoListaShared b));
int insertarListaShared(datoListaShared d,listashared* l);
int primeraListaShared (listashared l);
int ultimaListaShared (listashared l);
void vaciarListaShared(listashared *l);
void actualizarDatoShared(int pos, datoListaShared a,listashared*l);
