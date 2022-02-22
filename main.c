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



void set_config_defaults(struct ConnexionConfig * conf, int argc, char **argv) {
    conf->port=atoi(argv[argc-1]);
    conf->nb_mess = -1;
    conf->mode = NONE;
    conf->type = TCP;
    conf->url = NULL;
    conf->url_size = -1;
    conf->longueur_mess = 30;
}


int main(int argc, char **argv) {
    int c;
    extern char *optarg;
    extern int optind;

    struct ConnexionConfig conf;

    conf.port = atoi(argv[argc-1]); 
    conf.port = htons(conf.port);


    set_config_defaults(&conf, argc, argv);
    
    
	while ((c = getopt(argc, argv, "pn:usl:")) != -1) {

		switch (c) {
		case 'p': // puits 
			if ( conf.mode == Source ) {
				printf("usage: cmd [-p|-s][-n ##][-l ##]\n");
				exit(1);
			}
			conf.mode = Puits;
			break;

		case 's': // source
			if ( conf.mode == Puits ) {
				printf("usage: cmd [-p|-s][-n ##][-l ##]\n");
				exit(1) ;
			}
            conf.mode = Source;
            conf.url = argv[argc - 2];
            conf.url_size = strlen(conf.url);
			break;

		case 'n': // nombre de messages 
            conf.nb_mess = atoi(optarg);
			break;

        case 'u': // utilisation d'UDP 
            conf.type = UDP;
            break;

        case 'l': // longueur des messages 
            conf.longueur_mess = atoi(optarg);
   			break;

		default: // pas d'options 
			printf("usage: cmd [-p|-s][-n ##][-l ##]\n");
			break;
		}
	}

	// utilisation d'options sans préciser si mode source ou puits 
	if ( conf.mode == NONE ) {
		printf("usage: cmd [-p|-s][-n ##][-l ##]\n");
		exit(1) ;
	}
	
	// nombre de messages en mode source à 10 par défaut 
	if (conf.nb_mess == -1 && conf.mode == Source) {
		conf.nb_mess = 10 ; 
	}
	
	if ( conf.mode == Source ) {
        if ( conf.type == UDP ) {
			printf("SOURCE: long_ms_emis=%d, port=%d, nb_envois=%d, TP=%s, dest=%s\n",conf.longueur_mess,conf.port,conf.nb_mess,"UDP",conf.url);
        	UDP_source(&conf);
        }else{
        	printf("SOURCE: long_ms_emis=%d, port=%d, nb_envois=%d, TP=%s, dest=%s\n",conf.longueur_mess,conf.port,conf.nb_mess,"TCP",conf.url);
        	TCP_source(&conf);
        }
        printf("SOURCE: fin\n");
    } else { 
        if ( conf.type == UDP ) {
        	if ( conf.nb_mess == -1 ){
        		printf("PUITS: long_ms_lu=%d, port=%d, nb_receptions=infini, TP=%s\n",conf.longueur_mess,conf.port,"UDP");
        	}else{
        		printf("PUITS: long_ms_lu=%d, port=%d, nb_receptions=%d, TP=%s\n",conf.longueur_mess,conf.port,conf.nb_mess,"UDP");
        	}
            UDP_puits(&conf);
        } else {
        	if ( conf.nb_mess == -1 ){
        		printf("PUITS: long_ms_lu=%d, port=%d, nb_receptions=infini, TP=%s\n",conf.longueur_mess,conf.port,"TCP");
        	}else{
        		printf("PUITS: long_ms_lu=%d, port=%d, nb_receptions=%d, TP=%s\n",conf.longueur_mess,conf.port,conf.nb_mess,"TCP");
        	}
            TCP_puits(&conf);
        }
    }

	
}

