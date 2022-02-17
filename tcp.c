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



void send_TCP_message(int sk, struct ConnexionConfig * conf) {
    char * buff = malloc(sizeof(char)*conf->longueur_mess);
    int c = (int)'a';
    int env_res;

    for ( int n = 0; n < conf->nb_mess; n++ ) {
        create_message_line(buff, conf->longueur_mess,(char)(c + n));
        env_res = write(sk,buff,conf->longueur_mess*sizeof(char));
        printf("SOURCE: Envoi nº %d (%d) [%s]\n", n+1, conf->longueur_mess, buff);

        if ( env_res == -1 ) {
            printf("erreur à l'envoi du message");
            exit(-3);
        }
    }

    free(buff);
}

void TCP_source(struct ConnexionConfig * conf){
    int sk = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
		printf("erreur getbyhostname\n");
		exit(-2);
	}

    memcpy((char*)&adr.sin_addr.s_addr, hp->h_addr_list[0], hp->h_length);
    int connection = connect(sk,(struct sockaddr* )&adr, sizeof(adr));
    if ( connection == -1 ){
        printf("Erreur de connexion\n");
        exit(-3);
    }

    send_TCP_message(sk, conf);

}


// accept : "serveur"
void TCP_puits(struct ConnexionConfig * conf){
    int sk = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( sk == -1 ) {
        printf("Erreur pendant la creation du socket\n");
        exit(-1);
    }
    

    struct sockaddr_in adr;
    memset((char *) &adr, 0, sizeof(adr));

    adr.sin_family = AF_INET;
    adr.sin_port = htons(conf->port);
    adr.sin_addr.s_addr = INADDR_ANY;
    int binde = bind(sk, (struct sockaddr * ) & adr, sizeof(adr));
    if ( binde == -1){
        printf("Erreur lors du bind\n");
        exit(-4); 
    }
    
    struct sockaddr_in adr_client;
    int long_adr_client = sizeof(adr_client);
    listen(sk,1);
    int sock_connexion = accept(sk,(struct sockaddr*) &adr_client,(socklen_t *)&long_adr_client);
    if ( sock_connexion == -1 ){
        printf("Erreur dans l'appel à la primitive accept\n");
        exit(-5);
    }

    
    int max = 10 ; /* nb de messages attendus */
    int lg_max = 30 ; /* taille max de chaque message */
    char buff[lg_max];
    memset(buff, 0, lg_max);

    for (int i=0 ; i < max ; i ++) {
        int lg_rec = read(sock_connexion, buff, lg_max);
        if (lg_rec == -1){
            printf("Echec du read\n") ; exit(1) ;
        }
        afficher_message(buff,lg_rec, i, conf);
    }
}
