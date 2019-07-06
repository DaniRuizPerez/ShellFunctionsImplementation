/*Práctica de SO - Curso 2012/2013 de Segundo de GEI, FIC, UDC
Grupo 2.1 (viernes 8:30)
Autores: 
	- Ismael Barbeito Vázquez (i.barbeito@udc.es)
	- Daniel Ruiz Pérez (d.ruiz.perez@udc.es)

*/

#include "listamalloc.h"

void crearListaMalloc(listamalloc *l){
	l->ultima = -1;
}

int listaVaciaMalloc(listamalloc l){
	return (l.ultima == -1);
}

int buscarListaMalloc(datoListaMalloc d, listamalloc l, int (*comparar) (datoListaMalloc a, datoListaMalloc b)){
	int i = 0;
	while (i<l.ultima+1)
		if (comparar(d,l.datos[i]) == 0) 
			i++;
		else
			return i;
	return -1;
}

datoListaMalloc obtenerListaMalloc (int pos, listamalloc l) {
	return l.datos[pos];
}

int eliminarListaMalloc(datoListaMalloc x, listamalloc* l, int (*comparar) (datoListaMalloc a, datoListaMalloc b)) {
	int pos = buscarListaMalloc(x, *l, comparar);
	if (pos == -1)
		return 0;
	l->datos[pos] = l->datos[l->ultima];
	l->ultima--;
	return 1;
}

int insertarListaMalloc(datoListaMalloc d,listamalloc *l) {
	if (l->ultima == MAXTAM)
		return 0;
	l->ultima++;
	l->datos[l->ultima] = d;
	return 1;	 
}

int primeraListaMalloc (listamalloc l) {
	return 0;
}

int ultimaListaMalloc (listamalloc l) {
	return l.ultima;
}

void actualizarDato(int pos, datoListaMalloc a, listamalloc *l) {
	l->datos[pos] = a;
}

void vaciarListaMalloc(listamalloc *l) {
	l->ultima = -1;
}

