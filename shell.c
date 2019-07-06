/*Práctica de SO - Curso 2012/2013 de Segundo de GEI, FIC, UDC
Grupo 2.1 (viernes 8:30)
Autores: 
	- Ismael Barbeito Vázquez (i.barbeito@udc.es)
	- Daniel Ruiz Pérez (d.ruiz.perez@udc.es)

*/

#include "modulos/P2/P2.h"
#include "modulos/P3/listamalloc.h"
#include "modulos/P3/listammap.h"
#include "modulos/P3/listashared.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <ctype.h>


listamalloc lmalloc;
listashared lshared;
listammap lmmap;



/*AUXILIARES*/
/*-------------------------------------------------*/
int min (int a, int b) {
	return ((a<b)? a :b);
}

/*-------------------------------------------------*/
int compararTams (sshared a, sshared b) { //Comparar tam para shared
	return (a.tam == b.tam);
}
/*-------------------------------------------------*/
int compararTam (smalloc a, smalloc b) {
	return (a.tam == b.tam);
}
/*-------------------------------------------------*/
int compararName (smmap a, smmap b) {
	return (!strcmp(a.name, b.name));
}
/*-------------------------------------------------*/
void imprimirListaMalloc () {
	int i;
	smalloc aux;
	time_t tiempo;
	printf (" DIRECCION  TAMANO  TIEMPO \n");
	 
	for (i = primeraListaMalloc(lmalloc); i <= ultimaListaMalloc(lmalloc); i++) {
		aux = obtenerListaMalloc(i,lmalloc);
		tiempo = aux.time;
		printf (" %x  %d  %s\n",(int)aux.dir,aux.tam, strtok(ctime(&tiempo), "\n"));
		
	}
}
/*-------------------------------------------------*/
void imprimirListaMmap () {
	int i;
	smmap aux;
	time_t tiempo;
	printf (" DIRECCION  TAMANO  NOMBRE  FD  TIEMPO \n");
	 
	for (i = primeraListaMmap(lmmap); i <= ultimaListaMmap(lmmap); i++) {
		aux = obtenerListaMmap(i,lmmap);
		tiempo = aux.time;
		printf (" %x  %d %s %d  %s\n",(int)aux.dir,aux.tam, aux.name, aux.des, strtok(ctime(&tiempo), "\n"));
	}
}
/*-------------------------------------------------*/
void imprimirListaShared () {
	int i;
	sshared aux;
	time_t tiempo;
	printf (" DIRECCION  TAMANO  TIEMPO \n");
	 
	for (i = primeraListaShared(lshared); i <= ultimaListaShared(lshared); i++) {
		aux = obtenerListaShared(i,lshared);
		tiempo = aux.time;
		printf (" %x  %d  %s\n",(int)aux.dir,aux.tam, strtok(ctime(&tiempo), "\n"));
	}
}
/*-------------------------------------------------*/
smalloc crearSmalloc (void* x, int y) {
	smalloc a;
	a.dir = x;
	a.tam = y;
	a.time = time(NULL);
	return a;
}
/*-------------------------------------------------*/
sshared crearSshared(void* x, int y) {
	sshared a;
	a.dir = x;
	a.tam = y;
	a.time = time(NULL);
	return a;
}
/*-------------------------------------------------*/
smmap crearSmmap (void* x, int y, char*z, int b) {
	static smmap a;
	
	a.name = malloc (sizeof(char) * (strlen(z)+1) ); 
	a.dir = x;
	a.tam = y;
	sprintf (a.name, "%s", z);
	a.des = b;
	a.time = time(NULL);
	return a;
}
/*COMANDOS*/
/*-------------------------------------------------*/
void memalloc (char** trozos) {
	int tam;
	void* dir;
	if (trozos[1] == NULL || ((tam = atoi(trozos[1])) == 0)) 
		imprimirListaMalloc();
	else
		if ((dir=malloc(tam)) == NULL) 
			perror ("Imposible reservar memoria");
		else {
			insertarListaMalloc(crearSmalloc(dir,tam),&lmalloc);
			printf ("Reservados %d bytes en la direccion %x\n", tam, (int) dir);
		}
}
/*-------------------------------------------------*/
void freemem (char** stam) {
	int tam;
	int pos;
	smalloc a;

	if (stam[1] == NULL || ((tam = atoi(stam[1])) == 0)) 
		imprimirListaMalloc();
	else {
		a = crearSmalloc(0,tam);
		if ((pos = buscarListaMalloc(a, lmalloc, compararTam))==-1) 
			printf ("Error: no se ha encontrado el tamano indicado en la lista\n");
		else {
			a = obtenerListaMalloc(pos,lmalloc);
			printf ("Liberados %d bytes en la direccion %x\n", a.tam, (int) a.dir);
			free(a.dir);
			eliminarListaMalloc(a,&lmalloc, compararTam);
		}
	}
}
/*-------------------------------------------------*/
void memmap (char *fichero, char *tipomap, char * perm) {
	void *p;
	struct stat s;
	int df, protection=PROT_READ,map=MAP_PRIVATE,modo=O_RDONLY; 

	if (fichero==NULL) { 
		imprimirListaMmap(); 
		return;
	}
	if (tipomap!=NULL) {
		if (!strcmp(tipomap,"-s")) 
			map=MAP_SHARED;
		if (perm!=NULL && strlen(perm)<4) {
			protection=0;
			if (strchr(perm,'r')!=NULL) 
				protection|=PROT_READ;
			if (strchr(perm,'w')!=NULL) 
				protection|=PROT_WRITE;
			if (strchr(perm,'x')!=NULL) 
				protection|=PROT_EXEC;
		}
	}
	if (protection&PROT_WRITE) 
		modo=O_RDWR;
	if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1) { 
		perror ("Error: Imposible acceder al fichero"); 
		return; 
	}
	if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED) { 
		perror ("Error al mapear");
		close(df); 
		return; 
	}
	insertarListaMmap(crearSmmap(p,s.st_size,fichero,df),&lmmap);
	printf ("Fichero %s mapeado en %p\n", fichero, p);
	close(df);
}
/*-------------------------------------------------*/

void memunmap (char* fichero) {
	smmap a;
	int pos;
	
	if (fichero == NULL) 
		imprimirListaMmap();
	else {
		a = crearSmmap(0, 0, fichero, 0);
		if ((pos = buscarListaMmap(a, lmmap, compararName))==-1) 
			printf ("Error: no se ha encontrado el fichero indicado en la lista\n");
		else {
			a = obtenerListaMmap(pos,lmmap);
			if (munmap(a.dir, a.tam) == -1) 
				perror ("Error munmap");
			else {
				printf ("Desmapeado el fichero %s (direccion %x)\n", a.name, (int) a.dir);
				eliminarListaMmap(a,&lmmap, compararName);
			}
		}
	}
}
/*-------------------------------------------------*/
void * ObtenerMemoriaShmget (char* key, off_t tam) {
	void * p;
	int aux,id,flags=0777;
	key_t clave;
	struct shmid_ds s;
	
	if (key == NULL) {
		imprimirListaShared();
		return;
	}
	clave = atoi (key);
	if (tam)
		flags=flags | IPC_CREAT | IPC_EXCL;
	if (clave==IPC_PRIVATE) {
		errno=EINVAL; 
		return NULL;
	}
	if ((id=shmget(clave, tam, flags))==-1)
		return (NULL);
	if ((p=shmat(id,NULL,0))==(void*) -1){
		aux=errno;
		if (tam)
			shmctl(id,IPC_RMID,NULL);
		errno=aux;
		return (NULL);
	}
	shmctl (id,IPC_STAT,&s);
	insertarListaShared(crearSshared(p,tam),&lshared);
	return (p);
}
/*-------------------------------------------------*/

void munshared (char *param) {
	sshared a;
	int pos;
   int tam;
	
	if (param == NULL) 
		imprimirListaShared();
	else {
      tam = atoi(param);
		a = crearSshared(0,tam);
		if ((pos = buscarListaShared(a, lshared, compararTams))==-1) 
			printf ("Error: no se ha encontrado el tamaño indicado en la lista\n");
		else {
			a = obtenerListaShared(pos,lshared);
			if (shmdt (a.dir) == -1) 
				perror ("Error unshared");
			else {
				printf ("Desmapeada la memoria compartida de la direccion %x, con tamaño %d\n", (int) a.dir, (int)a.tam);
				eliminarListaShared(a,&lshared, compararTams);
			}
		}
	}
}
/*-------------------------------------------------*/
void mem (char** parametros) {
	int i = 1, s = 0, ma = 0, mm = 0;
	if (parametros[1] == NULL) {
		s = ma = mm = 1;
	}
	while (parametros[i] != NULL) {
		if (strstr(parametros[i], "-shared") != NULL) 
			s = 1;
		if (strstr(parametros[i], "-malloc") != NULL) 
			ma = 1;
		if (strstr(parametros[i], "-mmap") != NULL) 
			mm = 1;
		i++;
	}
	if (s){
		imprimirListaShared();
		printf("\n");
	}
	if (ma){
		imprimirListaMalloc();
		printf("\n");
	}
	if (mm) {
		imprimirListaMmap();
		printf("\n");
	}
}
/*-------------------------------------------------*/
void removekey (char * key) {
	key_t clave;
	int id;
		
	if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
		printf (" rmkey clave_valida\n");
		return;
	}
	if ((id=shmget(clave,0,0666))==-1){
		perror ("shmget: imposible obtener memoria compartida");
		return;
	}
	if (shmctl(id,IPC_RMID,NULL)==-1)
		perror ("shmctl: imposible eliminar memoria compartida\n");
	}
/*-------------------------------------------------*/
void recursiva (int n) {
	char automatico[MAXRUTA];
	static char estatico[MAXRUTA];
	
	printf ("Parametro n:%d en %p\n",n,&n);
	printf ("Array estatico en:%p \n",estatico);
	printf ("Array automatico en %p\n",automatico);
	if (n>0)
		recursiva(n-1);
}	

/*-------------------------------------------------*/
void memdump (char* direccion, char* cuantos) {
	
	char *dir = dir = (char*) strtoul(direccion, NULL, 16); //No se comprueba la direccion por petición explícita
	int numbytes = (cuantos==NULL)?25:atoi(cuantos);
	int i = 0,j;

	while (i < ((numbytes <= 25)?1:((numbytes % 25 == 0)?numbytes/25: numbytes/25+1))) {
		for (j = 0; j <min(25,(numbytes-i*25)); j++) {
			if (isprint(dir[j+i*25]))
				printf ("%2c ", dir[j+i*25]);
			else
				printf ("%2c ", ' ');
		}
		printf ("\n");
		for (j = 0; j< min(25,numbytes-i*25); j++) {
			if (isprint(dir[j+i*25]))
				printf ("%x ", dir[j+i*25]);
			else
				printf ("%x ", ' ');
		}
		printf ("\n");
		i++;
	}
}
/*-------------------------------------------------*/
void changeuid (int l, char* u) {

	char real[MAXRUTA], efectiva[MAXRUTA];
	uid_t nueva;
	
	if (!l)
		if (u == NULL) {
			strcpy(real,getpwuid(getuid())->pw_name);
			strcpy(efectiva,getpwuid(geteuid())->pw_name);
			printf("Credencial real:\n -login %s\n -numero %d\n -real %s\n",real, getuid(), getpwuid(getuid())->pw_gecos);
			printf("Credencial efectiva:\n -login %s\n -numero %d\n -real %s\n",efectiva, geteuid(), getpwuid(geteuid())->pw_gecos);
			return;
		}
		else 
			nueva = atoi(u);
	else
		if (u == NULL) {
			printf ("Error: No se ha especificado usuario\n");
			return;
		}
		else
			if (getpwnam(u)!= NULL) 
				nueva = getpwnam(u)->pw_uid;
			else {
				printf ("Error: error en el login indicado\n");
				return;
			}
	if (setuid(nueva) == -1) {
		if (errno == EAGAIN) {
			perror ("Error EAGAIN");
			return;
		}
		if (errno == EPERM) {
			perror ("Error EPERM");
			return;
		}
		perror ("Error desconocido");	
	}	
}

/*-------------------------------------------------*/
/*FUNCIONES DE INTERFAZ*/
/*-------------------------------------------------*/
void cmd_mmap(char**trozos) {
	int i = 2, tipo = 0, perm = 0;
	
	if (trozos[1] != NULL)
		while (trozos[i]!= NULL) {
			if (!strcmp(trozos[i],"-s"))
				tipo = i;
			else
				perm = i;
			i++;	
		}
	memmap (trozos[1], (!tipo)?NULL:trozos[tipo],(!perm)?NULL:trozos[perm]);
}
/*-------------------------------------------------*/
void cmd_changeuid(char* l, char* u){
	
	if (l == NULL)
		changeuid (0,NULL);
	else if (!strcmp (l, "-l"))
		changeuid (1, u);
	else 
		changeuid(0, l);		
}

/*PARTE PRINCIPAL DEL SHELL*/
/*-------------------------------------------------*/
int cantidadParam (int tiene, int cuantos) {
	int a = (tiene <= cuantos);
		if (!a) 
			printf ("Número de parámetros inválido\n");
	return a;
}
/*-------------------------------------------------*/
void Selector (char* trozos[], int numtrozos) {
/*Este procedimiento comprueba la existencia de parámetros y,
según el contenido de trozos[0], llama a la función adecuada.*/

//SHARED strtoul NULL 10

	if (trozos[0] != NULL) {
		if (!strcmp(trozos[0], "q")) {
			exit(0);
		}
		if (!strcmp(trozos[0], "quit")) {
			exit(0);
		}
		if (!strcmp(trozos[0], "fin")) {
			exit(0);
		}
		if (!strcmp(trozos[0], "exit")) {
			exit(0);
		}
		if (!strcmp(trozos[0], "pid")) {
			pid();
			return;
		}
		if (!strcmp(trozos[0], "pwd")) {
			pwd();
			return;
		}
		if (!strcmp(trozos[0], "chdir")) {
			if (cantidadParam (numtrozos, 2)) 
				changedir (trozos[1]);
			return;
		}
		if (!strcmp(trozos[0], "list")) {
			if (cantidadParam (numtrozos, 5)) 
				cmd_list (trozos);
			return;
		}
		if (!strcmp(trozos[0], "delete")) {
			if (cantidadParam (numtrozos, 2)) 
				delete (trozos[1]);
			return;
		}
		if (!strcmp(trozos[0], "deltree")) {
			if (cantidadParam (numtrozos, 2)) 
				cmd_deltree (trozos[1]); 
			return;
		}
		if (!strcmp(trozos[0], "cls")) {
			system("clear");
			return;
		}
		if (!strcmp(trozos[0], "clear")) {
			system("clear");
			return;
		}
		if (!strcmp(trozos[0], "prio")) {
			prio(trozos);
			return;
		}
		if (!strcmp(trozos[0], "fork")) {
			fornikation();
			return;
		}
		if (!strcmp(trozos[0], "ejecutar")) {
			ejecutar(trozos, numtrozos);
			return;
		}
		if (!strcmp(trozos[0], "splano")) {
			splano(trozos, numtrozos);
			return;
		}
		if (!strcmp(trozos[0], "procesos")) {
			if (cantidadParam (numtrozos, 2)) 
				procesos (trozos); 
			return;
		}
		if (!strcmp(trozos[0], "borraprocesos")) {
			if (cantidadParam (numtrozos, 2)) 
				cmd_borraprocesos (trozos); 
			return;
		}
		if (!strcmp(trozos[0], "infoproc")) {
			if (cantidadParam (numtrozos, 2)) 
				cmd_infoproc(trozos); 
			return;
		}
		if (!strcmp(trozos[0], "pplano")) {
			if (cantidadParam (numtrozos, 2)) 
				pplano (trozos); 
			return;
		}
		if (!strcmp(trozos[0], "malloc")) {
			memalloc(trozos);
			return;
		}
		if (!strcmp(trozos[0], "free")) {
			freemem(trozos);
			return;
		}
		if (!strcmp(trozos[0], "mem")) {
			mem(trozos);
			return;
		}
		if (!strcmp(trozos[0], "mmap")) {
			cmd_mmap(trozos);
			return;
		}
		if (!strcmp(trozos[0], "munmap")) {
			memunmap(trozos[1]);
			return;
		}
		if (!strcmp(trozos[0], "shared")) {
			ObtenerMemoriaShmget (trozos[1], (trozos[1] !=NULL && trozos[2] != NULL)?atoi(trozos[2]):0);
			return;
		}
		if (!strcmp(trozos[0], "rmkey")) {
			removekey(trozos[1]);
			return;
		}
		if (!strcmp(trozos[0], "recursiva")) {
			recursiva((trozos[1]==NULL)?0:atoi(trozos[1]));
			return;
		}
		if (!strcmp(trozos[0], "memdump")) {
			memdump(trozos[1],(trozos[1]==NULL)?NULL:trozos[2]);
			return;
		}		
      if (!strcmp(trozos[0], "unshared")) {
			munshared(trozos[1]);
			return;
		}
		if (!strcmp(trozos[0], "uid")) {
			cmd_changeuid(trozos[1], (trozos[1]!=NULL)?trozos[2]:NULL);
			return;
		}		
		if (!strcmp(trozos[0], "autores")) {
			printf ("Hecho por: \n Ismael Barbeito VÃƒÂ¡zquez\n Daniel Ruiz PÃƒÂ©rez \nCurso 2012/2013, Segundo GEI, grupo 2.1\n");
			return;
		}
		ejecutarpp(trozos,numtrozos);
	}
}
/*-------------------------------------------------*/

int TrocearCadena (char* cadena, char*trozos[]) {
/*Cada posición del vector "trozos" pasa a contener
un puntero a cada uno de los parámetros separados por
" ", "\n" o "\t" en la cadena original. Delimitado por un
puntero a NULL*/
	int i = 1;
	
	if ((trozos[0] = strtok(cadena, " \n\t")) == NULL) {
		return 0;
	}
	while ((trozos[i] = strtok (NULL, " \n\t"))!= NULL) {
		i++;
	}
	return i;;
}
/*-------------------------------------------------*/
int main (void) {
/*Incialización gráfica, captura de la cadena de entrada y llamada al selector*/
	char cadena[MAXENTRADA];
	char *trozos[MAXENTRADA];
	int numtrozos;
	
	system ("clear");
	printf ("Inciando shell...\n");
	cmd_crearListaProc();
	crearListaShared(&lshared);	
	crearListaMalloc(&lmalloc);
	crearListaMmap(&lmmap);
	while (1) {
		printf("-> ");
		fgets (cadena,MAXENTRADA,stdin);
		numtrozos = TrocearCadena(cadena, trozos);
		Selector(trozos,numtrozos);
	}
	return 0;
}
