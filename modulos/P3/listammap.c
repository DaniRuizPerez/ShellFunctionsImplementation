/*Práctica de SO - Curso 2012/2013 de Segundo de GEI, FIC, UDC
Grupo 2.1 (viernes 8:30)
Autores: 
	- Ismael Barbeito Vázquez (i.barbeito@udc.es)
	- Daniel Ruiz Pérez (d.ruiz.perez@udc.es)

*/

#include "listammap.h"

void crearListaMmap(listammap *l){
	l->ultima = -1;
}

int listaVaciaMmap(listammap l){
	return (l.ultima == -1);
}

int buscarListaMmap(datoListaMmap d, listammap l, int (*comparar) (datoListaMmap a, datoListaMmap b)){
	int i = 0;
	while (i<l.ultima+1)
		if (comparar(d,l.datos[i]) == 0) 
			i++;
		else
			return i;
	return -1;
}

datoListaMmap obtenerListaMmap (int pos, listammap l) {
	return l.datos[pos];
}

int eliminarListaMmap(datoListaMmap x, listammap* l, int (*comparar) (datoListaMmap a, datoListaMmap b)) {
	int pos = buscarListaMmap(x, *l, comparar);
	if (pos == -1)
		return 0;
	l->datos[pos] = l->datos[l->ultima];
	l->ultima--;
	return 1;
}

int insertarListaMmap(datoListaMmap d,listammap *l) {
	if (l->ultima == MAXTAM)
		return 0;
	l->ultima++;
	l->datos[l->ultima] = d;
	return 1;	 
}

int primeraListaMmap (listammap l) {
	return 0;
}

int ultimaListaMmap (listammap l) {
	return l.ultima;
}

void actualizarDatoMmap(int pos, datoListaMmap a, listammap *l) {
	l->datos[pos] = a;
}

void vaciarListaMmap(listammap *l) {
	l->ultima = -1;
}

