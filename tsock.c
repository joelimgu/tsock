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

#define NONE 0
#define todo printf("Not implemented yet\n");exit(-1);


enum bool {false, true};
typedef enum bool bool;

enum ConnexionType {UDP=1, TCP}; // | NONE
typedef enum ConnexionType ConnexionType;

enum ConnexionMode {Puits=1, Source}; // | NONE
typedef enum ConnexionMode ConnexionMode;

struct ConnexionConfig {
    int port;
    ConnexionType type; // TCP | UDP
    ConnexionMode mode; // source | puits
    char * url;
    int url_size;
    int nb_mess;
    int longueur_mess;
};


/*
    @param creates a message and writes it in the provided adress
    @param mess_len 
    @param conf
*/
void create_message_line(char * mess, int mess_len, char c, struct ConnexionConfig * conf) {
    for ( int i = 0; i < mess_len; i++ ) {
        mess[i] = c;
    }
}


/*
    @param conf is a read only reference, it does not modify it's content. Note: conf.port must be passed with C's default integer binaryu representation the funcion will perform the necessary conversions
*/
void UDP_source(struct ConnexionConfig * conf) {
	int sk = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if ( sk == -1 ) {
		printf("Erreur pendant la creation du socket\n");
		exit(-1);
	}
	printf("on a bien créé le socket\n");
	struct sockaddr_in adr;
	memset((char*)&adr,0,sizeof(adr));

	adr.sin_family = AF_INET;
	adr.sin_port = htons(conf->port); // todo take argument

	struct hostent *hp = gethostbyname(conf->url); // todo take argument
	if ( hp == NULL ) {
		printf("erreur getbyhostname\n");
		exit(1);
	}
	memcpy((char*)&adr.sin_addr.s_addr, hp->h_addr_list[0], hp->h_length);
	printf("on a bien créé l'adresse\n");
	char buff[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	int msg_s = sendto(sk,buff,sizeof(buff)/sizeof(buff[0]), 0, (struct sockaddr* )&adr, sizeof(adr));
	printf("send %d\n",msg_s);
	close(sk);
	printf("send aaaaaaaaaaaaaaaaa\n");
}


/*
    @param conf is a read only reference, it does not modify it's content. Note: conf.port must be passed with C's default integer binaryu representation the funcion will perform the necessary conversions
*/
void UDP_puits(struct ConnexionConfig * conf) {
    char buff[1024];
    memset(buff, 0, 1024);

    int sk = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if ( sk == -1 ) {
        printf("Erreur pendant la creation du socket\n");
        exit(-1);
    }
    printf("on a bien créé le socket\n");
    struct sockaddr_in adr;
    memset((char *) &adr, 0, sizeof(adr));

    adr.sin_family = AF_INET;
    adr.sin_port = htons(5565); // todo take argument
    adr.sin_addr.s_addr = INADDR_ANY;
    bind(sk, (struct sockaddr * ) & adr, sizeof(adr));

    struct sockaddr * adr_emeteur = NULL;
    int p_long_adr_emeteur = sizeof(adr);
    while ( true ) {
        recvfrom(sk, buff, 1024, 0, adr_emeteur, (socklen_t *)&p_long_adr_emeteur);
        printf("%s\n", buff);
    }
    close(sk); // ça ser à rien yass todo close socket
}


/*
    @param conf is a read only reference, it does not modify it's content. Note: conf.port must be passed with C's default integer binaryu representation the funcion will perform the necessary conversions
*/
void TCP_source(struct ConnexionConfig * conf) {
    todo
}

/*
    @param conf is a read only reference, it does not modify it's content. Note: conf.port must be passed with C's default integer binaryu representation the funcion will perform the necessary conversions
*/
void TCP_puits(struct ConnexionConfig * conf) {
    todo
}


void set_config_defaults(struct ConnexionConfig * conf, int argc, char **argv) {
    conf->port = atoi(argv[argc-1]); // todo tester cas erreur utilisateur
    // conf->port = htons(conf->port); // pas necessaire

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
            // printf("set url to %s, wiht len %d", conf.url, conf.url_size);
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

