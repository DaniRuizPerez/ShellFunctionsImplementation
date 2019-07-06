#include "../P1/P1.h"
#include "listaproc.h"
#include <sys/wait.h>
/*AUXILIARES*/
void cmd_crearListaProc ();
int compararPID (sproc a, sproc b);
int compararEstado (sproc a, sproc b);
sproc crearSproc (int pid, char** com);
void actualizarProceso (sproc *aux, int i, int a);
void actualizarListaProc();
void imprimirListaProc (char* opcion);
void prio (char ** trozos);
void fornikation();
void ejecutar(char** trozos, int numtrozos);
void ejecutarpp(char** trozos, int numtrozos);
void splano (char** trozos, int numtrozos);
void procesos (char** trozos);
void infoproc (sproc aux);
void pplano (char**trozos);
void borraprocesos (char*opcion);
void cmd_borraprocesos (char**trozos);
void cmd_infoproc (char**trozos);
