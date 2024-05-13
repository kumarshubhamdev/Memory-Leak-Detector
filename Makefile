TARGET:exe
exe: main.o   lib.a
	gcc main.o -o exe  -L . lib.a

lib.a:mld.o
	ar rs  lib.a  mld.o 


 
mld.o:mld.c
	gcc -c mld.c -o mld.o


main.o:main.c
	gcc -c main.c -o main.o