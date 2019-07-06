/*Práctica de SO - Curso 2012/2013 de Segundo de GEI, FIC, UDC
Grupo 2.1 (viernes 8:30)
Autores: 
	- Ismael Barbeito Vázquez (i.barbeito@udc.es)
	- Daniel Ruiz Pérez (d.ruiz.perez@udc.es)

*/

#include "listashared.h"

void crearListaShared(listashared *l){
	l->ultima = -1;
}

int listaVaciaShared(listashared l){
	return (l.ultima == -1);
}

int buscarListaShared(datoListaShared d, listashared l, int (*comparar) (datoListaShared a, datoListaShared b)){
	int i = 0;
	while (i<l.ultima+1)
		if (comparar(d,l.datos[i]) == 0) 
			i++;
		else
			return i;
	return -1;
}

datoListaShared obtenerListaShared (int pos, listashared l) {
	return l.datos[pos];
}

int eliminarListaShared(datoListaShared x, listashared* l, int (*comparar) (datoListaShared a, datoListaShared b)) {
	int pos = buscarListaShared(x, *l, comparar);
	if (pos == -1)
		return 0;
	l->datos[pos] = l->datos[l->ultima];
	l->ultima--;
	return 1;
}

int insertarListaShared(datoListaShared d,listashared *l) {
	if (l->ultima == MAXTAM)
		return 0;
	l->ultima++;
	l->datos[l->ultima] = d;
	return 1;	 
}

int primeraListaShared (listashared l) {
	return 0;
}

int ultimaListaShared (listashared l) {
	return l.ultima;
}

void actualizarDatoShared(int pos, datoListaShared a, listashared *l) {
	l->datos[pos] = a;
}

void vaciarListaShared(listashared *l) {
	l->ultima = -1;
}

