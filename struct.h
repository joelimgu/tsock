#define NONE 0
#define todo printf("Not implemented yet\n");exit(-1);

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

void send_TCP_message(int sk, struct ConnexionConfig * conf);

void TCP_source(struct ConnexionConfig * conf);

void TCP_puits(struct ConnexionConfig * conf);

void send_UDP_message(int sk, struct sockaddr* adr, struct ConnexionConfig * conf);

void UDP_source(struct ConnexionConfig * conf);

void UDP_puits(struct ConnexionConfig * conf);
