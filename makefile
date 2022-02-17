tsock : main.o tcp.o udp.o
	gcc -o tsock main.o tcp.o udp.o


main.o : main.c
	gcc -c -Wall main.c


tcp.o : tcp.c
	gcc -c -Wall tcp.c


udp.o : udp.c
	gcc -c -Wall udp.c


clean :
	rm -f *.o
