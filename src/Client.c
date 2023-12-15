#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* pour memset */
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h> /* pour struct sockaddr_in */
#include <arpa/inet.h> /* pour htons et inet_aton */

#define LONGUEUR_TEXTE 500

int main(int argc, char *argv[]) {
    if (argc != 4) {
        perror("Nombre d'arguments incorrecte.");
        exit(EXIT_FAILURE);
    }

    int socket_d;
    char messageEnvoye[LONGUEUR_TEXTE];
    char messageRecus[LONGUEUR_TEXTE];
    struct sockaddr_in pointDistant;
    socklen_t longueurAdresse;
    
    if ((socket_d = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erreur lors de la création du descripteur socket.");
        exit(EXIT_FAILURE);
    }
    
    longueurAdresse = sizeof(pointDistant);
    memset(&pointDistant, 0x00, longueurAdresse);
    
    // Renseigne la structure sockaddr_in avec les informations du serveur distant AF > PF
    pointDistant.sin_family = PF_INET;
    // On choisit le numéro de port d’écoute du serveur
    pointDistant.sin_port = htons(atoi(argv[2])); // htons(IPPORT_USERRESERVED); // = 5000
    // On choisit l’adresse IPv4 du serveur
    inet_aton(argv[1], &pointDistant.sin_addr); // à modifier
    
    if (
        (connect(
            socket_d,
            (struct sockaddr*) &pointDistant,
            longueurAdresse
        )) == -1
    ) {
        perror("Erreur lors de la connection"); // Affiche le message d’erreur
        close(socket_d); // On ferme la ressource avant de quitter
        exit(-2); // On sort en indiquant un code erreur
    }
    
    printf("Connexion réussie !\n");
    
    memset(messageEnvoye, 0x00, LONGUEUR_TEXTE * sizeof(char));
    memset(messageRecus, 0x00, LONGUEUR_TEXTE * sizeof(char));
    
    sprintf(messageEnvoye, argv[3]);
    
    switch (write(socket_d, messageEnvoye, strlen(messageEnvoye))) {
    case -1:
        perror("Message non envoyé.");
        close(socket_d);
        exit(-3);
    case 0:
        fprintf(stderr, "Le socket a été fermé par le serveur.");
        close(socket_d);
        return 0;
    default:
        printf("Message envoyé.\n");
    }
    
    switch (read(socket_d, messageRecus, LONGUEUR_TEXTE * sizeof(char))) {
    case -1:
        perror("Message non reçus.");
        close(socket_d);
        exit(-4);
    case 0:
        fprintf(stderr, "Le socket a été fermé par le serveur.");
        close(socket_d);
        return 0;
    default:
        printf("Message reçus : %s\n.", messageRecus);
    }
    
    close(socket_d);

    return EXIT_SUCCESS;
}

