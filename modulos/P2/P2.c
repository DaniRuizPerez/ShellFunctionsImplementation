
#include "P2.h"
/*AUXILIARES*/
listaProc lp; //usada como variable global por ahorrar pasos de parámetros

/*-------------------------------------------------*/
void cmd_crearListaProc () {
	crearListaProc (&lp);
}
/*-------------------------------------------------*/
int compararPID (sproc a, sproc b) {
	return (a.pid ==b.pid);
}
/*-------------------------------------------------*/
int compararEstado (sproc a, sproc b) {
	return (!strcmp(a.estado,b.estado));
}
/*-------------------------------------------------*/
sproc crearSproc (int pid, char** com) {
	static sproc a;
	int i = 1;


	a.pid = pid;
	a.time = time(NULL);
	sprintf (a.estado,"act");
	sprintf (a.comando,"");
	while (com[i] != NULL) {
		if (com[i][0] != '@')
			sprintf (a.comando,"%s %s",a.comando, com[i]);
		i++;
	}
	a.causa = 0;
	return a;
}
/*-------------------------------------------------*/
void actualizarProceso (sproc *aux, int i, int a) {
/*Supongo que la ListaProcProc no está vacía*/	
	int estado;
	
		if (wait4 (aux->pid, &estado, (a? 0:WNOHANG|WUNTRACED|WCONTINUED), &(aux->ru)) == aux->pid) {
			if(WIFEXITED(estado)) {
				sprintf(aux->estado, "term");
				aux->causa = WEXITSTATUS(estado);
			}
			if(WIFSIGNALED(estado)) {
				sprintf(aux->estado, "sig");
				aux->causa =  WTERMSIG(estado);
			}
			if(WIFSTOPPED(estado)) {
				sprintf(aux->estado, "stop");
				aux->causa =  WSTOPSIG(estado);
			}
			if(WIFCONTINUED(estado)) {
				sprintf(aux->estado, "act");
				aux->causa =  0;
			}
		}
		actualizarDatoProc(i,*aux,&lp);
}

/*-------------------------------------------------*/
void actualizarListaProc() {
	sproc aux;
	int i;
	
	for (i = primeraListaProc(lp); i <= ultimaListaProc(lp); i++) {
		aux = obtenerListaProc(i,lp);
		actualizarProceso(&aux,i,0);
	}
}

/*-------------------------------------------------*/
void imprimirListaProc (char* opcion) {
	int i;
	sproc aux;
	time_t tiempo;

	printf (" PID  PRIO  COMANDO ESTADO TIEMPO \n");
	 
		for (i = primeraListaProc(lp); i <= ultimaListaProc(lp); i++) {
			aux = obtenerListaProc(i,lp);
			tiempo = aux.time;
			if (!strcmp(opcion,aux.estado) || (!strcmp(opcion, "all"))) 
				printf (" %d   %d %8s   %4s   %s\n",aux.pid, getpriority(PRIO_PROCESS, aux.pid), aux.comando, aux.estado, strtok(ctime(&tiempo), "\n"));
		}
}


/*COMANDOS*/

void prio (char ** trozos) {
	if (trozos[1] == NULL) {
		printf("%d\n",getpriority(PRIO_PROCESS,0));
	} 
	else {
		if (trozos[2] == NULL) {
			printf("%d\n",getpriority(PRIO_PROCESS,atoi(trozos[1])));
		}
		else {
			printf("%s\n",((setpriority(PRIO_PROCESS, atoi(trozos[1]), atoi(trozos[2])))?"Error al cambiar la prioridad":"Prioridad cambiada con éxito"));
		}
	}
}

/*-------------------------------------------------*/
void fornikation() {
	pid_t pid;
	int estado;

	if ((pid=fork())==0) {
		printf("Estamos en el hijo...\n");
	} 
	else
		if (pid < 0) {
			perror ("No se ha podido crear un hijo del shell\n");
			return;
		}
		else {
			waitpid(pid, &estado, 0); //Preguntar si waitpid puede fallar si fork no falla (¿fork dando pid inválido?)
			printf("Volviendo al shell...\n");
		}
}
/*-------------------------------------------------*/
void ejecutar(char** trozos, int numtrozos) {

	char* parametros[numtrozos-2];
	int i, prio;

	if (numtrozos == 1) {
		printf ("Error: ninguna dirección indicada\n");
		return;
	}
	if (trozos[numtrozos-1][0] == '@') {
		prio = atoi (trozos[numtrozos-1]+1);
		numtrozos-=2;
		setpriority(PRIO_PROCESS,0, prio);
	}
	
	for (i=1; i <= numtrozos;i++) {
		parametros[i-1] = trozos[i];
	}
	parametros[numtrozos] = NULL;
	execvp(trozos[1],parametros);
	perror("Error:");
}
/*-------------------------------------------------*/
void ejecutarpp(char** trozos, int numtrozos){
	pid_t pid;
	int i,prio, estado;
	
	if ((pid=fork())==0) { //hijo 
		if (trozos[numtrozos-1][0] == '@') {
			prio = atoi (trozos[numtrozos-1]+1);
			setpriority(PRIO_PROCESS,getpid(), prio);
			trozos[numtrozos-1] = NULL;
		}
		execvp (trozos[0],trozos);
		perror ("No se ha podido ejecutar el programa");
		exit(-1);
	} 
	else 
		if (pid < 0) {//fork falla
			perror ("No se ha podido ejecutar (error al crear proceso)\n");
			return;
		}
		else {//padre
			waitpid(pid, &estado, 0);
		}
	
}

/*-------------------------------------------------*/
void splano (char** trozos, int numtrozos){
	pid_t pid;
	int i, estado;
	
	if ((pid=fork())==0) { //hijo 
		ejecutar(trozos,numtrozos);
		printf("-> ");
		exit(-1);
	} 
	else 
		if (pid < 0) {//fork falla
			perror ("No se ha podido ejecutar (error al crear proceso)\n");
			return;
		}
		
		else
			insertarListaProc(crearSproc(pid,trozos), &lp);
}

/*-------------------------------------------------*/
void procesos (char** trozos) {
	char* opcion = malloc(100);

	if (trozos[1] == NULL)
		sprintf (opcion, "all");
	else
		sprintf(opcion,"%s",trozos[1]);
	actualizarListaProc();
	imprimirListaProc(opcion);
	free(opcion);
}
/*-------------------------------------------------*/
void infoproc (sproc aux) {
	time_t tiempo;
	tiempo = aux.time;
	printf ("\nPid: %d\nPrioridad: %d\nComando: %s\nFecha/hora: %s\nEstado: %s\nCausa: %d\n",aux.pid, getpriority(PRIO_PROCESS,aux.pid), aux.comando, strtok(ctime(&tiempo), "\n"), aux.estado,aux.causa);
		if (!strcmp (aux.estado,"sig") || !strcmp(aux.estado,"term"))  {
			printf("\n\nRecursos usados\n-----------------------\nTiempo CPU (ms): usuario=%.3f; sistema=%.3f\n",
				      aux.ru.ru_utime.tv_sec + aux.ru.ru_utime.tv_usec / 1000.0,
				      aux.ru.ru_stime.tv_sec + aux.ru.ru_stime.tv_usec / 1000.0);
			printf("Tamaño máximo residente %ld\n", aux.ru.ru_maxrss);
			printf("Memoria compartida: %ld\n", aux.ru.ru_ixrss);
			printf("Datos no-compartidos: %ld\n", aux.ru.ru_idrss);
			printf("Stack no compartido: %ld\n", aux.ru.ru_isrss);
			printf("Peticiones de página: %ld\n", aux.ru.ru_minflt);
			printf("Errores de página: %ld\n", aux.ru.ru_majflt);
			printf("Swaps: %ld\n", aux.ru.ru_nswap);
			printf("Bloques entrada/salida: entrada=%ld; salida=%ld\n",aux.ru.ru_inblock, aux.ru.ru_oublock);
			printf("Señales recibidas: %ld\n", aux.ru.ru_nsignals);
			printf("Mensajes IPC: enviados=%ld; recibidos=%ld\n", aux.ru.ru_msgsnd, aux.ru.ru_msgrcv);
			printf("Cambios de contexto: voluntarios=%ld; "
				      "involuntarios=%ld\n", aux.ru.ru_nvcsw, aux.ru.ru_nivcsw);
		}
}
/*-------------------------------------------------*/
void pplano (char**trozos) {
	sproc a;
	int pos,estado;

	if (trozos[1] == NULL) {
		printf ("Error: no se ha especificado un pid\n");
		return;	
	}
	a.pid = atoi(trozos[1]);
	if ((pos = buscarListaProc(a, lp, compararPID)) == -1)  {
		printf("Error: el PID especificado no ha sido encontrado en la ListaProcProc de procesos\n");	
		return;
	}
	a= obtenerListaProc(pos,lp);
	actualizarProceso(&a,pos,1);
	infoproc(a);	
}

/*-------------------------------------------------*/
void borraprocesos (char*opcion) {
	sproc a;

	actualizarListaProc(lp);
	if (!strcmp (opcion, "all")) {
		vaciarListaProc(&lp);
		return;
	}
	sprintf (a.estado, "%s", opcion);
	while (eliminarListaProc (a, &lp, compararEstado) == 1);
}

/*FUNCIONES DE INTERFAZ*/
/*-------------------------------------------------*/
void cmd_borraprocesos (char**trozos) {

	if ((trozos[1] == NULL)||(strcmp(trozos[1],"all") && strcmp(trozos[1],"term") && strcmp(trozos[1],"sig") &&strcmp(trozos[1],"stop") 
	&&strcmp(trozos[1],"act"))) { //funciona por evaluación perezosa
		imprimirListaProc("all");
		return;	
	}
	if (listaVaciaProc(lp)) {
		printf ("Error: la ListaProcProc de procesos está vacía");
		return;
	}
	borraprocesos (trozos[1]);
}
/*-------------------------------------------------*/
void cmd_infoproc (char**trozos) {
	sproc a;
	struct rusage r;
	int pos;
	int estado;
	
	if (trozos[1] == NULL) {
		printf("Error: Ningún PID especificado");
		return;
	}
	a.pid = atoi(trozos[1]);
	if ((pos =buscarListaProc(a,lp,compararPID))==-1) {
		printf("Error: No se ha encontrado el proceso en la ListaProcProc\n");
		return;
	}

	a= obtenerListaProc(pos,lp);
	actualizarProceso(&a,pos,0);
	infoproc(a);
	
}

