/* librairie standard ... */
#include <stdlib.h>
/* pour getopt */
#include <unistd.h>
/* déclaration des types de base */
#include <sys/types.h>
/* constantes relatives aux domaines, types et protocoles */
#include <sys/socket.h>
/* constantes et structures propres au domaine UNIX */
#include <sys/un.h>
/* constantes et structures propres au domaine INTERNET */
#include <netinet/in.h>
/* structures retournées par les fonctions de gestion de la base de
données du réseau */
#include <netdb.h>
/* pour les entrées/sorties */
#include <stdio.h>
/* pour la gestion des erreurs */
#include <errno.h>

enum bool {false, true};
enum ConnexionType {None, UDP, TCP};
typedef enum bool bool;


int main(int argc, char **argv) {
	int c;
	extern char *optarg;
	extern int optind;
	int nb_message = -1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
	int source = -1 ; /* 0=puits, 1=source */
    enum ConnexionType connexion = None;
    int port = atoi(argv[argc-1]); // todo tester cas erreur utilisateur
    port = htons(port);

	while ((c = getopt(argc, argv, "pn:su")) != -1) {
		switch (c) {
		case 'p':
			if (source == 1) {
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1);
			}
			source = 0;
			break;

		case 's':
			if (source == 0) {
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1) ;
			}
			source = 1;
			break;

		case 'n':
			nb_message = atoi(optarg);
			break;

        case 'u':
            connexion = UDP;
            break;

		default:
			printf("usage: cmd [-p|-s][-n ##]\n");
			break;
		}
	}

	if (source == -1) {
		printf("usage: cmd [-p|-s][-n ##]\n");
		exit(1) ;
	}

	if (source == 1)
		printf("on est dans le source\n");
        if ( connexion == UDP ) {
            int sk = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if ( sk == -1 ) {
                printf("Erreur pendant la creation du socket\n");
                exit(-1);
            }
            printf("on a bien créé le socket\n");
            struct sockaddr_in adr;
            adr.sin_family = AF_INET;
            adr.sin_port = 5565; // todo take argument

            struct hostent *hp = gethostbyname("insa-20161"); // todo take argument
            if ( hp == NULL ) {
                printf("erreur getbyhostname\n");
                exit(1);
            }
            memcpy((char*)&adr.sin_addr.s_addr, hp->h_addr_list[0], hp->h_length);
            printf("on a bien créé l'adresse\n");
            char buff[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
            sendto(sk,buff,sizeof(buff)/sizeof(buff[0]), 0, (struct sockaddr* )&adr, sizeof(adr));
            close(sk);
            printf("send aaaaaaaaaaaaaaaaa\n");
        }

	else
		printf("on est dans le puits\n");

	if (nb_message != -1) {
		if (source == 1)
			printf("nb de tampons à envoyer : %d\n", nb_message);
		else
			printf("nb de tampons à recevoir : %d\n", nb_message);
	} else {
		if (source == 1) {
			nb_message = 10 ;
			printf("nb de tampons à envoyer = 10 par défaut\n");
		} else
		printf("nb de tampons à envoyer = infini\n");

	}
}
