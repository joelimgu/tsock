tsock : main.o tcp.o udp.o messages.o
	gcc -o tsock main.o tcp.o udp.o messages.o


main.o : main.c
	gcc -c -Wall main.c


tcp.o : tcp.c
	gcc -c -Wall tcp.c


udp.o : udp.c
	gcc -c -Wall udp.c

messages.o : messages.c
	gcc -c -Wall messages.c

clean :
	rm -f *.o
	rm -f *.gch
