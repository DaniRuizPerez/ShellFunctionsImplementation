clean: shell.out
	rm modulos/P1/P1.o modulos/P2/listaproc.o modulos/P2/P2.o modulos/P3/listammap.o modulos/P3/listamalloc.o modulos/P3/listashared.o

shell.out: shell.c modulos/P1/P1.h modulos/P1/P1.o modulos/P2/listaproc.o modulos/P2/listaproc.h modulos/P2/P2.h modulos/P2/P2.o modulos/P3/listamalloc.h modulos/P3/listamalloc.o modulos/P3/listammap.h modulos/P3/listammap.o modulos/P3/listashared.h modulos/P3/listashared.o
	gcc -o shell.out shell.c modulos/P1/P1.o modulos/P2/P2.o modulos/P2/listaproc.o modulos/P3/listashared.o modulos/P3/listamalloc.o modulos/P3/listammap.o

P1.o: modulos/P1/P1.h modulos/P1/P1.c
	gcc -c modulos/P1/P1.c

listaproc.o: modulos/P2/listaproc.c modulos/P2/listaproc.h modulos/P2/datoproc.h
	gcc -c modulos/P2/listaproc.c

P2.o: modulos/P2/P2.h modulos/P2/P2.c
	gcc -c modulos/P2/P2.c

listamalloc.o: modulos/P2/listamalloc.c modulos/P2/listamalloc.h modulos/P2/datomalloc.h
	gcc -c modulos/P2/listamalloc.c
	
listammap.o: modulos/P2/listammap.c modulos/P2/listammap.h modulos/P2/datommap.h
	gcc -c modulos/P2/listammap.c

listashared.o: modulos/P2/listashared.c modulos/P2/listashared.h modulos/P2/datoshared.h
	gcc -c modulos/P2/listashared.c
	


