tsock : main.o tcp.o udp.o affichage.o
	gcc -o tsock main.o tcp.o udp.o affichage.o


main.o : main.c
	gcc -c -Wall main.c


tcp.o : tcp.c
	gcc -c -Wall tcp.c


udp.o : udp.c
	gcc -c -Wall udp.c

affichage.o : affichage.c
	gcc -c -Wall affichage.c

clean :
	rm -f *.o
	rm -f *.gch
