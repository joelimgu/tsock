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


// transformer num en un tableau contenant les codes ascii de ses chiffres 
int * int_to_code_ascii(int num){
	int * tab_num = malloc(5*sizeof(int)); // tableau de taille maximum 5  
	int code_0 = (int)'0' ; 
	int n = num ; 
	int reste = 0 ; 
	for (int i = 0 ; i < 5 ; i ++) {
		reste = n%10 ; 
		if ( i != 0 && reste == 0 ) {
			tab_num[4-i]=(int)'-'; // les premiers caractères sont des tirets 
		}else{
			tab_num[4-i]=code_0+reste;
		}
		n = n/10; 
	}
	return tab_num ; 
}

void create_message_line(int num, char * mess, int mess_len, char c) {
	int * tab_num = int_to_code_ascii(num) ; 
    for (int i = 0 ; i < 5 ; i ++) {
    	mess[i]=(char)tab_num[i]; 
    }
    for ( int i = 5; i < mess_len-5; i++ ) {
        mess[i] = c;
    }
}
