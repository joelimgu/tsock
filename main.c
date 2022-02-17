/* librairie standard ... */
#include <stdlib.h>
/* pour getopt */
#include <unistd.h>
/* déclaration des types de base */
#include <sys/types.h>
/* pour les entrées/sorties */
#include <stdio.h>
/* pour la gestion des erreurs */
#include <errno.h>
/* structures et types */ 
#include <struct.h>



void set_config_defaults(struct ConnexionConfig * conf, int argc, char **argv) {
    conf->port = atoi(argv[argc-1]); // todo tester cas erreur utilisateur

    conf->nb_mess = 10;
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

    conf.port = atoi(argv[argc-1]); // todo tester cas erreur utilisateur
    conf.port = htons(conf.port);

    conf.nb_mess = -1;
    conf.mode = NONE;
    conf.type = NONE;
    conf.url = NULL;
    conf.url_size = -1;



    set_config_defaults(&conf, argc, argv);
    
	while ((c = getopt(argc, argv, "pn:usl:")) != -1) {

		switch (c) {
		case 'p':
			if ( conf.mode == Source ) {
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1);
			}
			conf.mode = Puits;
			break;

		case 's': // source
			if ( conf.mode == Puits ) {
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1) ;
			}
            conf.mode = Source;
            conf.url = argv[argc - 2];
            conf.url_size = strlen(conf.url);
			break;

		case 'n':
            conf.nb_mess = atoi(optarg);
			break;

        case 'u':
            conf.type = UDP;
            break;

        case 'l':
            conf.longueur_mess = atoi(optarg);
            printf("long mess: %d\n", conf.longueur_mess);

		default:
			printf("usage: cmd [-p|-s][-n ##]\n");
			break;
		}
	}

	if ( conf.mode == NONE ) {
		printf("usage: cmd [-p|-s][-n ##]\n");
		exit(1) ;
	}

	if ( conf.mode == Source ) {
        printf("on est dans le source\n");
        if ( conf.type == UDP ) {
            UDP_source(&conf);
        } else {
            TCP_source(&conf);
        }
    } else { // recieve
        printf("on est dans le puits\n");
        if ( conf.type == UDP ) {
            UDP_puits(&conf);
        } else {
            TCP_puits(&conf);
        }
    }

	if (conf.nb_mess != -1) {
		if ( conf.mode == Source ) {
            printf("nb de tampons à envoyer : %d\n", conf.nb_mess);
        } else {
            printf("nb de tampons à recevoir : %d\n", conf.nb_mess);
        }
	} else {
		if ( conf.mode == Source ) {
			conf.nb_mess = 10 ;
			printf("nb de tampons à envoyer = 10 par défaut\n");
		} else {
            printf("nb de tampons à recevoir = infini\n");
        }
	}
}

