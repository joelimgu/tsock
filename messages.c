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


void afficher_message(char *message, int lg, int nb_mess, struct ConnexionConfig * conf) {
    int i;
    printf("PUITS: Reception nº %d (%d) [",nb_mess, conf->longueur_mess);
    for (i=0;i<lg;i++){
        printf("%c", message[i]);
    }
    printf("]\n");
}

void create_message_line(char * mess, int mess_len, char c) {
    for ( int i = 0; i < mess_len; i++ ) {
        mess[i] = c;
    }
}