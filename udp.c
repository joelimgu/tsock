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
/* structures et types */ 
#include "struct.h"



void send_UDP_message(int sk, struct sockaddr* adr, struct ConnexionConfig * conf) {
    char * buff = malloc(sizeof(char)*conf->longueur_mess);
    int c = (int)'a';
    
    for ( int n = 0; n < conf->nb_mess; n++ ) {
        create_message_line(buff, conf->longueur_mess,(char)(c + n));
        sendto(sk,buff,conf->longueur_mess*sizeof(char), 0, (struct sockaddr* )adr, sizeof(struct sockaddr_in));
        printf("SOURCE: Envoi nº %d (%d) [%s]\n", n+1, conf->longueur_mess, buff);
    }

    free(buff);
}

void UDP_source(struct ConnexionConfig * conf) {
	int sk = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if ( sk == -1 ) {
		printf("Erreur pendant la creation du socket\n");
		exit(-1);
	}
	struct sockaddr_in adr;
	memset((char*)&adr,0,sizeof(adr));

	adr.sin_family = AF_INET;
	adr.sin_port = htons(conf->port); 

	struct hostent *hp = gethostbyname(conf->url);
	if ( hp == NULL ) {
		printf("Erreur getbyhostname\n");
		exit(-2);
	}
	memcpy((char*)&adr.sin_addr.s_addr, hp->h_addr_list[0], hp->h_length);


    send_UDP_message(sk, (struct sockaddr* )&adr, conf);

	close(sk);
}

void UDP_puits(struct ConnexionConfig * conf) {
    char buff[1024];
    int mess_counter = 0;
    memset(buff, 0, 1024);

    int sk = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if ( sk == -1 ) {
        printf("Erreur pendant la creation du socket\n");
        exit(-1);
    }
    struct sockaddr_in adr;
    memset((char *) &adr, 0, sizeof(adr));

    adr.sin_family = AF_INET;
    adr.sin_port = htons(conf->port);
    adr.sin_addr.s_addr = INADDR_ANY;
    bind(sk, (struct sockaddr * ) & adr, sizeof(adr));

    struct sockaddr * adr_emeteur = NULL;
    int p_long_adr_emeteur = sizeof(adr);

    while ( true ) {
        recvfrom(sk, buff, 1024, 0, adr_emeteur,(socklen_t *)&p_long_adr_emeteur);
        mess_counter++;
        printf("PUITS: Reception nº %d (%d) [%s]\n",mess_counter, conf->longueur_mess, buff);
    }
    close(sk); // inaccesible code
}

