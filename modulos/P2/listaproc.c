/*Práctica de SO - Curso 2012/2013 de Segundo de GEI, FIC, UDC
Grupo 2.1 (viernes 8:30)
Autores: 
	- Ismael Barbeito Vázquez (i.barbeito@udc.es)
	- Daniel Ruiz Pérez (d.ruiz.perez@udc.es)

*/

#include "listaproc.h"

void crearListaProc(listaProc *l){
	l->ultima = -1;
}

int listaVaciaProc(listaProc l){
	return (l.ultima == -1);
}

int buscarListaProc(datoListaProc d, listaProc l, int (*comparar) (datoListaProc a, datoListaProc b)){
	int i = 0;
	while (i<l.ultima+1)
		if (comparar(d,l.datos[i]) == 0) 
			i++;
		else
			return i;
	return -1;
}

datoListaProc obtenerListaProc (int pos, listaProc l) {
	return l.datos[pos];
}

int eliminarListaProc(datoListaProc x, listaProc* l, int (*comparar) (datoListaProc a, datoListaProc b)) {
	int pos = buscarListaProc(x, *l, comparar);
	if (pos == -1)
		return 0;
	l->datos[pos] = l->datos[l->ultima];
	l->ultima--;
	return 1;
}

int insertarListaProc(datoListaProc d,listaProc *l) {
	if (l->ultima == MAXTAM)
		return 0;
	l->ultima++;
	l->datos[l->ultima] = d;
	return 1;	 
}

int primeraListaProc (listaProc l) {
	return 0;
}

int ultimaListaProc (listaProc l) {
	return l.ultima;
}

void actualizarDatoProc(int pos, datoListaProc a, listaProc *l) {
	l->datos[pos] = a;
}

void vaciarListaProc(listaProc *l) {
	l->ultima = -1;
}

