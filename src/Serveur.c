#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "Matiere.h"
#include "Message.h"

struct sockaddr_in creerServeur(short family, uint16_t port) {
	struct sockaddr_in serveur;
	serveur.sin_family = PF_INET;
	serveur.sin_addr.s_addr = htonl(INADDR_ANY);
	serveur.sin_port = htons(port);

	return serveur;
}

/*int tailleTableauMatiere(Matiere matieres[]) {
	return sizeof(matieres) / sizeof(Matiere);
}*/

float trouverMoyenne(int id, Matiere matieres[]){
	for (int i = 0; i < 6; i++) {
		if (matieres[i].id == id) {
			return matieres[i].moyenne;
		}
	}
	return -1;
}

int main() {
	Matiere matieres[] = {
		{ 1,"Anglais\0", 12.5},
		{ 2,"Mathematiques\0", 10.8},
		{ 3,"Informatique\0", 9.3},
		{ 4,"Culturte generale\0", 12.6},
		{ 5,"Physiques\0", 11.8},
		{ 6,"Programmation systeme\0", 12.3}
	};
	//printf("moyenne = %f\n", trouverMoyenne(3, TabMatieres));
	
	int socketEcoute;
	int socketDialogue;
	struct sockaddr_in serveur;
	struct sockaddr_in pointDeRencontreDistant;

	socklen_t adresseLongueur = sizeof(struct sockaddr_in);

	char messageEnvoi[LONGUEUR_MESSAGE];
	char messageRecu[LONGUEUR_MESSAGE];	
	
	// Crée un socket de communication
	if ((socketEcoute = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Erreur lors de la création du socket\n");
		return EXIT_FAILURE;
	}
	
	serveur = creerServeur(PF_INET, IPPORT_USERRESERVED);

	// On demande l’attachement local au socket
	if ((bind(socketEcoute, (struct sockaddr*) &serveur, adresseLongueur)) < 0) {
		perror("Erreur lors de l'attachement local\n");
		return EXIT_FAILURE;
	}

	// On fixe la taille de la file d’attente à 5 (pour les demandes de connexion non encore traitées)
	if (listen(socketEcoute, 5) < 0) {
		perror("Erreur lors de l'écoute\n");
		return EXIT_FAILURE;
	}

	// On attend une connexion
	while (1) {
		printf("Attente d'une demande de connexion (quitter avec Ctrl-C)\n\n");

		// Création du socket de dialogue
		if ((socketDialogue = accept(socketEcoute, (struct sockaddr*) &pointDeRencontreDistant, &adresseLongueur)) < 0) {
			perror("Erreur lors de la cration du socket de dialogue\n");
			close(socketDialogue);
			close(socketEcoute);

			return EXIT_FAILURE;
		}

		// On reçoit le message
		memset(messageRecu, 0x00, LONGUEUR_MESSAGE * sizeof(char));
		if (!recevoirMessage(socketDialogue, messageRecu, LONGUEUR_MESSAGE * sizeof(char))) {
			return EXIT_FAILURE;
		}
		
		// On écrit le message à envoyer
		memset(messageEnvoi, 0x00, LONGUEUR_MESSAGE * sizeof(char));
		sprintf(messageEnvoi, "%f\n", trouverMoyenne(atoi(messageRecu), matieres));

		// On envoie le message
		if (!envoyerMessage(socketDialogue, messageEnvoi, strlen(messageEnvoi))) {
			return EXIT_FAILURE;
		}

		sleep(5);

		close(socketDialogue);
	}

	close(socketEcoute);

	return EXIT_SUCCESS;
}
