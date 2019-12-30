all:	controleHoras
controleHoras:	registro.o	main.o
	gcc -o ./out/controlehoras ./out/registro.o ./out/main.o -lreadline
registro.o:	registro.c
	gcc -o ./out/registro.o -c registro.c -W -Wall
main.o:	RegistrarHoras.c	registro.h
	gcc -o ./out/main.o -c RegistrarHoras.c -W -lreadline
clean:
	rm out/*

