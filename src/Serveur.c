#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "Message.h"

#define NB_MATIERE 6

typedef struct Matiere_s {
    int id;
    char nom[50];
    float moyenne;
} Matiere;

struct sockaddr_in creerServeur(short family, uint16_t port) {
    struct sockaddr_in serveur;

    serveur.sin_family = PF_INET;
    serveur.sin_addr.s_addr = htonl(INADDR_ANY);
    serveur.sin_port = htons(port);

    return serveur;
}

float trouverMoyenne(int id, Matiere matieres[]){
    for (int i = 0; i < NB_MATIERE; i++) {
        if (matieres[i].id == id) {
            return matieres[i].moyenne;
        }
    }
    return -1;
}

int main(void) {
    Matiere matieres[NB_MATIERE] = {
        { 1, "Anglais\0", 12.5 },
        { 2, "Mathematiques\0", 10.8 },
        { 3, "Informatique\0", 9.3 },
        { 4, "Culturte generale\0", 12.6 },
        { 5, "Physiques\0", 11.8 },
        { 6, "Programmation systeme\0", 12.3 }
    };

    int socketEcoute;
    int socketDialogue;
    socklen_t adresseLongueur;
    struct sockaddr_in serveur;

    char messageEnvoye[LONGUEUR_MESSAGE];
    char messageRecu[LONGUEUR_MESSAGE];	

    // Création d'un scoket d'écoute
    if ((socketEcoute = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Erreur lors de la création du socket\n");
        
        return EXIT_FAILURE;
    }

    // Création d'un serveur
    serveur = creerServeur(PF_INET, IPPORT_USERRESERVED);

    // Attachement local
    adresseLongueur = sizeof(struct sockaddr_in);

    if ((bind(socketEcoute, (struct sockaddr*) &serveur, adresseLongueur)) < 0) {
        fprintf(stderr, "Erreur lors de l'attachement local\n");
        
        return EXIT_FAILURE;
    }

    // On fixe la taille de la file d’attente à 5
    if (listen(socketEcoute, 5) < 0) {
        fprintf(stderr, "Erreur lors de l'écoute\n");
        
        return EXIT_FAILURE;
    }

    // On attend une connexion
    while (1) {
        printf("Attente d'une demande de connexion (quitter avec Ctrl-C)\n\n");

        // Création du socket de dialogue
        if ((socketDialogue = accept(socketEcoute, (struct sockaddr*) &serveur, &adresseLongueur)) < 0) {
            fprintf(stderr, "Erreur lors de la cration du socket de dialogue\n");
            close(socketDialogue);
            close(socketEcoute);

            return EXIT_FAILURE;
        }

        // On reçoit le message
        memset(messageRecu, 0x00, LONGUEUR_MESSAGE * sizeof(char));
        if (!recevoirMessage(socketDialogue, messageRecu, LONGUEUR_MESSAGE * sizeof(char))) {
            close(socketDialogue);
            
            return EXIT_FAILURE;
        }

        // On écrit le message à envoyer
        memset(messageEnvoye, 0x00, LONGUEUR_MESSAGE * sizeof(char));
        sprintf(messageEnvoye, "%f\n", trouverMoyenne(atoi(messageRecu), matieres));

        // On envoie le message
        if (!envoyerMessage(socketDialogue, messageEnvoye, strlen(messageEnvoye))) {
            close(socketDialogue);
            
            return EXIT_FAILURE;
        }

        // On ferme le socket de dialogue
        close(socketDialogue);
    }

    // On ferme le socket d'écoute
    close(socketEcoute);

    return EXIT_SUCCESS;
}
