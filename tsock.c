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


void afficher_message(char *message, int lg) {
    int i;
    printf("message construit : ");
    for (i=0;i<lg;i++){
        printf("%c", message[i]); printf("\n");
    } 
}



/*
    @param creates a message and writes it in the provided adress
    @param mess_len 
    @param conf
*/
void create_message_line(char * mess, int mess_len, char c) {
    for ( int i = 0; i < mess_len; i++ ) {
        mess[i] = c;
    }
}


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
	adr.sin_port = htons(conf->port); 

	struct hostent *hp = gethostbyname(conf->url);
	if ( hp == NULL ) {
		printf("erreur getbyhostname\n");
		exit(1);
	}
	memcpy((char*)&adr.sin_addr.s_addr, hp->h_addr_list[0], hp->h_length);
	printf("on a bien créé l'adresse\n");

    send_UDP_message(sk, (struct sockaddr* )&adr, conf);

	// char buff[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	// int msg_s = sendto(sk,buff,sizeof(buff)/sizeof(buff[0]), 0, (struct sockaddr* )&adr, sizeof(adr));
	// printf("send %d\n",msg_s);
	close(sk);
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
    adr.sin_port = htons(conf->port); // todo take argument
    adr.sin_addr.s_addr = INADDR_ANY;
    bind(sk, (struct sockaddr * ) & adr, sizeof(adr));

    struct sockaddr * adr_emeteur = NULL;
    int p_long_adr_emeteur = sizeof(adr);
    while ( true ) {
        recvfrom(sk, buff, 1024, 0, adr_emeteur,(socklen_t *)&p_long_adr_emeteur);
        printf("%s\n", buff);
    }
    close(sk); // ça ser à rien yass todo close socket
}


// connect : "client"
void TCP_source(struct ConnexionConfig * conf){
    int sk = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
    int connection = connect(sk,(struct sockaddr* )&adr, sizeof(adr));
    if ( connection == -1 ){
        printf("erreur de connexion\n");
        exit(-2);
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
    printf("on a bien créé le socket\n");

    struct sockaddr_in adr;
    memset((char *) &adr, 0, sizeof(adr));

    adr.sin_family = AF_INET;
    adr.sin_port = htons(conf->port); // todo take argument
    adr.sin_addr.s_addr = INADDR_ANY;
    int binde = bind(sk, (struct sockaddr * ) & adr, sizeof(adr));
    if ( binde == -1){
        printf("erreur lors du bind\n");
        exit(-4); // todo uniformiser les codes d'erreur?
    }


    struct sockaddr_in adr_client;
    int long_adr_client = sizeof(adr_client);
    listen(sk,1);
    int sock_connexion = accept(sk,(struct sockaddr*) &adr_client,(socklen_t *)&long_adr_client);
    if ( sock_connexion == -1 ){
        printf("erreur dans l'appel à la primitive accept\n");
        perror("Error");
        exit(-5);
    }

    
    int max = 10 ; /* nb de messages attendus */
    int lg_max = 30 ; /* taille max de chaque message */
    char buff[lg_max];
    memset(buff, 0, lg_max);

    for (int i=0 ; i < max ; i ++) {
        int lg_rec = read(sock_connexion, buff, lg_max);
        if (lg_rec == -1){
            printf("échec du read\n") ; exit(1) ;
        }
        afficher_message(buff,lg_rec);
    }
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

    conf.port = atoi(argv[argc-1]); // todo tester cas erreur utilisateur
    conf.port = htons(conf.port);

    conf.nb_mess = -1;
    conf.mode = NONE;
    conf.type = NONE;
    conf.url = NULL;
    conf.url_size = -1;

//	int nb_message = -1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
//	int source = -1 ; /* 0=puits, 1=source */
//    enum ConnexionType connexion = NONE;


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

