/*Práctica de SO - Curso 2012/2013 de Segundo de GEI, FIC, UDC
Grupo 2.1 (viernes 8:30)
Autores: 
	- Ismael Barbeito Vázquez (i.barbeito@udc.es)
	- Daniel Ruiz Pérez (d.ruiz.perez@udc.es)

*/

/*FUNCIONES AUXILIARES*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <time.h>


#define MAXENTRADA 200
#define MAXRUTA 4096

char* CadenaPermisos (struct stat s);
/*Obtiene una cadena que contiene, al estilo del "ls -li", los permisos
del fichero que tiene por stat a "s"*/	


char* ObtenerNombre (int nombre);
/*Convierte el id (tanto de grupo como de usuario) "nombre" en un string*/


char* ObtenerFecha (struct stat s); 
/*Convierte en string una fecha de un directorio (en concreto última modificación) a partir
de su variable stat. Formato de la fecha: aaaa-mm-dd hh:mm (año-mes-día hora:minuto)*/


void ImprimirInfoArchivo (char *dir); 
/*Imprime la información del directorio de la ruta dada. 
NOTA: no imprime el nombre del directorio en si, solo la información 
restante del listado largo */


int esDirectorio(char * dir);
/*Comprueba si la ruta dada en el string "dir" se corresponde a un directorio.
Importante pasarle una ruta, debido a que usa la función stat*/

int existeDir (char * dir);
/*Comprueba si la ruta dada en el string "dir" existe como directorio.
Se basa en la llamada "opendir", así que no precisa la ruta completa*/


/*Funciones que realizan la gestión de los comandos del shell*/

void pid (); 

void pwd (); 

void changedir (char ruta[]); 

void list (int l, int h, char * dir); 
	
void listr (int l, int h, char * dir); 

void delete (char archivo[]);

int deltree (char archivo[]); 


/*Funciones de interfaz*/

void cmd_list (char *parametros[]); 
	/*Hace las comprobaciones básicas para el comando "list" y sus opciones.
	Así mismo, llama a una de las dos subfunciones (la del listado "normal" y la del
	listado recursivo)*/


void cmd_deltree (char * archivo);
/*Hace comprobaciones de existencia de
archivo para deltree, así como comprueba
que la ejecución del mismo ha sido
exitosa.*/

