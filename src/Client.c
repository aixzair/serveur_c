#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* pour memset */
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h> /* pour struct sockaddr_in */
#include <arpa/inet.h> /* pour htons et inet_aton */

#include "Message.h"

#define LONGUEUR_TEXTE 500

int main(int argc, char *argv[]) {
    if (argc != 4) {
        perror("Nombre d'arguments incorrecte.");
        exit(EXIT_FAILURE);
    }

    int socket_d;
    char message[LONGUEUR_TEXTE];
    struct sockaddr_in pointDistant;
    socklen_t adresseLongueur;
    
    if ((socket_d = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erreur lors de la création du descripteur socket.");
        exit(EXIT_FAILURE);
    }
    
    adresseLongueur = sizeof(pointDistant);
    memset(&pointDistant, 0x00, adresseLongueur);
    
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
            adresseLongueur
        )) == -1
    ) {
        perror("Erreur lors de la connection"); // Affiche le message d’erreur
        close(socket_d); // On ferme la ressource avant de quitter
        exit(-2); // On sort en indiquant un code erreur
    }
    
    printf("Connexion réussie !\n");

    // Envoie le message au serveur
    memset(message, 0x00, LONGUEUR_TEXTE * sizeof(char));
    sprintf(message, argv[3]);

    if (!envoyerMessage(socket_d, message, strlen(message))) {
        return EXIT_FAILURE;
    }
    
    // Reçoit le message
    memset(message, 0x00, LONGUEUR_TEXTE * sizeof(char));

    if (!recevoirMessage(socket_d, message, LONGUEUR_MESSAGE * sizeof(char))) {
        return EXIT_FAILURE;
    }

    printf("Message reçus : %.2f.\n", strtof(message, NULL));

    sleep(1);

    close(socket_d);

    return EXIT_SUCCESS;
}
