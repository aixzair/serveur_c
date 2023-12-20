#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "Message.h"

#define LONGUEUR_TEXTE 500

/**
 * @brief créer un serveur
 * 
 * @param ip adresse ip du serveur
 * @param port port du serveur
 * @return struct sockaddr_in
 */
struct sockaddr_in creerServeur(const char *ip, uint16_t port) {
    struct sockaddr_in serveur;
    memset(&seveur, 0x00, sizeof(serveur));

    serveur.sin_family = PF_INET;
    serveur.sin_port = htons(port);
    inet_aton(ip, &serveur.sin_addr);

    return serveur;
}

int main(int argc, char *argv[]) {
    int socketServeur;
    struct sockaddr_in serveur;

    char message[LONGUEUR_TEXTE];

    // Vérification des paramètres
    if (argc != 4) {
        perror("Nombre d'arguments incorrecte.");
        return EXIT_FAILURE;
    }
    
    // Création du socket
    if ((socketServeur = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erreur lors de la création du socket de communication.");

        return EXIT_FAILURE;
    }
    
    // Création du serveur
    serveur = creerServeur(argv[1],(uint16_t) atoi(argv[2]));
    
    // Connection au serveur par le socket
    if ((connect(
            socketServeur,
            (struct sockaddr*) &server,
            sizeof(serveur)
        )) == -1
    ) {
        perror("Erreur lors de la connection");
        close(socketServeur);

        return EXIT_FAILURE;
    } else {
        printf("Connexion réussie !\n");
    }

    // Envoie le message au serveur
    memset(message, 0x00, LONGUEUR_TEXTE * sizeof(char));
    sprintf(message, argv[3]);

    if (!envoyerMessage(socketServeur, message, strlen(message))) {
        return EXIT_FAILURE;
    }
    
    // Reçoit le message
    memset(message, 0x00, LONGUEUR_TEXTE * sizeof(char));
    if (!recevoirMessage(socketServeur, message, LONGUEUR_MESSAGE * sizeof(char))) {
        return EXIT_FAILURE;
    }

    // Ferme la socket
    close(socketServeur);

    // Affiche le message reçus
    printf("Message reçus : %.2f.\n", strtof(message, NULL));

    return EXIT_SUCCESS;
}
