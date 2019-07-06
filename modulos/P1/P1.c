/*Práctica de SO - Curso 2012/2013 de Segundo de GEI, FIC, UDC
Grupo 2.1 (viernes 8:30)
Autores: 
	- Ismael Barbeito Vázquez (i.barbeito@udc.es)
	- Daniel Ruiz Pérez (d.ruiz.perez@udc.es)

*/

/*FUNCIONES AUXILIARES*/


#include "P1.h"

char* CadenaPermisos (struct stat s) {
/*Obtiene una cadena que contiene, al estilo del "ls -li", los permisos
del fichero que tiene por stat a "s"*/	
	
	static char permisos[11]; 
	
	permisos [0] = (S_ISDIR(s.st_mode)) ? 'd' : '-';
	permisos [1] = (s.st_mode & S_IRUSR) ? 'r' : '-';
   permisos [2] = (s.st_mode & S_IWUSR) ? 'w' : '-';
   permisos [3] = (s.st_mode & S_IXUSR) ? 'x' : '-';
   permisos [4] = (s.st_mode & S_IRGRP) ? 'r' : '-';
   permisos [5] = (s.st_mode & S_IWGRP) ? 'w' : '-';
  	permisos [6] = (s.st_mode & S_IXGRP) ? 'x' : '-';
  	permisos [7] = (s.st_mode & S_IROTH) ? 'r' : '-';
  	permisos [8] = (s.st_mode & S_IWOTH) ? 'w' : '-';
  	permisos [9] = (s.st_mode & S_IXOTH) ? 'x' : '-';
	permisos [10] = '\0';
	return permisos;
}

char* ObtenerNombre (int nombre) {
/*Convierte el id (tanto de grupo como de usuario) "nombre" en un string*/
	struct passwd *pw;
		if ((pw=getpwuid(nombre)) == NULL) {
			perror ("Imposible obtener nombre");
			return "(null)";
		}
	return pw->pw_name;
}

char* ObtenerFecha (struct stat s) {
/*Convierte en string una fecha de un directorio (en concreto última modificación) a partir
de su variable stat. Formato de la fecha: aaaa-mm-dd hh:mm (año-mes-día hora:minuto)*/
	static char fechora[80];
	struct tm *tmP;
	if ((tmP = localtime(&s.st_mtime)) == NULL) {
		perror ("Imposible obtener fecha y hora");
		return "(null)";
	}
	if ((strftime(fechora, 80, "%Y-%m-%d %H:%M", tmP)) == 0) {
		perror ("Imposible obtener fecha y hora");
		return "(null)";
	}//0 no sería siempre error de usar %p
	return fechora;
}

void ImprimirInfoArchivo (char *dir)  {
/*Imprime la información del directorio de la ruta dada. 
NOTA: no imprime el nombre del directorio en si, solo la información 
restante del listado largo */
   struct stat s;
	if (stat(dir,&s) < 0) {
		perror ("Imposible listar");
		return;
	}
	printf ("%d %s %d %s %s %7d %s ",(int)s.st_ino, CadenaPermisos (s), (int)s.st_nlink, ObtenerNombre (s.st_uid), ObtenerNombre (s.st_gid),(int) s.st_size, ObtenerFecha (s));			

}

int esDirectorio(char * dir)
/*Comprueba si la ruta dada en el string "dir" se corresponde a un directorio.
Importante pasarle una ruta, debido a que usa la función stat*/
{
  struct stat s;
  if (stat(dir,&s)==-1)
		return 0;
  return (S_ISDIR(s.st_mode));
}

int existeDir (char * dir)
/*Comprueba si la ruta dada en el string "dir" existe como directorio.
Se basa en la llamada "opendir", así que no precisa la ruta completa*/
{
	DIR *dirp;
	struct dirent *direntp;

	if ((dirp = opendir(dir))== NULL) {
		closedir (dirp);
		return 0;
	}
	closedir (dirp);
	return 1;
}

/*Funciones que realizan la gestión de los comandos del shell*/

void pid () {
	printf ("shell: %d\nbash: %d\n",getpid(),getppid());
}

void pwd () {
	char ruta[MAXRUTA];
	if (getcwd (ruta,MAXRUTA)==NULL)
		perror("Imposible obtener directorio");
	else printf ("%s\n", ruta);
}

void changedir (char ruta[]) {
	if (ruta==NULL)
		pwd();
	else if (chdir (ruta) == -1) {
			perror("chdir");
	}
}

void list (int l, int h, char * dir) {
	DIR *dirp;
	char rutatemp [MAXRUTA];
	struct dirent *direntp;
	
	if ((dirp = opendir(dir)) == NULL) {
		perror ("No se pudo abrir el directorio");
		return;
	}
	while ((direntp = readdir(dirp)) != NULL) {
		if (direntp->d_name[0] == '.' && h)
			continue;
		if ((strcmp(direntp->d_name, ".")) && (strcmp(direntp->d_name, ".."))) {
			if (!l) {
				printf ("%s\n", direntp->d_name);
			}
			else {
				sprintf(rutatemp,"%s/%s",dir, direntp->d_name);
				ImprimirInfoArchivo (rutatemp);
				printf ("%s\n", direntp->d_name);
			}
		}
	}
	closedir(dirp);
}

void listr (int l, int h, char * dir) {
	DIR *dirp;
	struct dirent *direntp;
	char rutatemp [MAXRUTA];
	
	printf ("\nListando %s:\n\n", dir);
	list (l, h, dir);
	if ((dirp = opendir(dir)) == NULL) {
		perror ("No se pudo abrir el directorio");
		return;
	}
	while ((direntp = readdir(dirp)) != NULL) {
		if (!(strcmp(direntp->d_name, ".")) || (!strcmp(direntp->d_name, "..")))
			continue;
		sprintf(rutatemp,"%s/%s",dir, direntp->d_name);
		if (esDirectorio (rutatemp)) {
			listr (l, h,rutatemp);
			
		}
	}
	closedir(dirp);
	
}

void delete (char archivo[]) {
	if (unlink (archivo) == -1) {
		perror ("delete");
	}
}

int deltree (char archivo[]) {
	char rutatemp [MAXRUTA];
	DIR *dirp;
	struct dirent *direntp;

	if ((dirp = opendir(archivo))== NULL) 
		return -1;
	while ((direntp = readdir(dirp)) != NULL) {
		if (!strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, ".."))
				continue; 
		sprintf(rutatemp,"%s/%s",archivo, direntp->d_name);
		if (esDirectorio(rutatemp)) {
			if (deltree(rutatemp)==-1)  
				return -1;
		}else 
			if (unlink(rutatemp)==-1) 
				return -1;
	}
	closedir(dirp);
	return rmdir (archivo);
}


/*Funciones de interfaz*/

void cmd_list (char *parametros[]) {
	/*Hace las comprobaciones básicas para el comando "list" y sus opciones.
	Así mismo, llama a una de las dos subfunciones (la del listado "normal" y la del
	listado recursivo)*/
	int i = 1, l = 0, r = 0, h = 0;
	char dir [MAXRUTA] = "";
	char rutaactual [MAXRUTA];

	while (parametros [i] != NULL) {
		if (!strcmp (parametros[i], "-l"))
			l = 1;
		else if (!strcmp (parametros[i], "-r"))
			r = 1;
		else if (!strcmp (parametros[i], "-h"))
			h = 1;
		else strcpy (dir, parametros[i]);
		i++;
	}
	if (strcmp(dir, "")) {
		if (!existeDir(dir)) {
			perror ("Parámetro inválido (¿dir existente? ¿Opción correcta?)");
			return;
		}

	}
	else 
		getcwd (dir, MAXRUTA);
	(r)?listr (l, h, dir):list (l, h, dir);
}

void cmd_deltree (char * archivo){
/*Hace comprobaciones de existencia de
archivo para deltree, así como comprueba
que la ejecución del mismo ha sido
exitosa.*/
  	if (archivo==NULL){
    	printf ("deltree: No address\n");
		return;
   }
	if (deltree(archivo)==-1)
		perror ("Imposible borrar directorio");
}
