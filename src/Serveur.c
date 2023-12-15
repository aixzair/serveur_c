#include <stdio.h>
#include <stdlib.h> /* pour exit */
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h> /* pour memset */
#include <netinet/in.h> /* pour struct sockaddr_in */
#include <arpa/inet.h> /* pour htons et inet_aton */
#include <unistd.h> /* pour sleep */
#include "Matiere.h"
#define PORT IPPORT_USERRESERVED // = 5000
#define LG_MESSAGE 256

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
	struct sockaddr_in pointDeRencontreLocal;
	socklen_t longueurAdresse;
	int socketDialogue;
	struct sockaddr_in pointDeRencontreDistant;
	char messageEnvoi[LG_MESSAGE];
	char messageRecu[LG_MESSAGE];
	int ecrits, lus; /* nb d’octets ecrits et lus */
	
	
	// Crée un socket de communication
	socketEcoute = socket(PF_INET, SOCK_STREAM, 0); 
	
	// Teste la valeur renvoyée par l’appel système socket()
	if (socketEcoute < 0) {
		perror("socket"); // Affiche le message d’erreur
		exit(-1);
	}
	
	
	printf("Socket créé avec succès ! (%d)\n", socketEcoute);
	// On prépare l’adresse d’attachement locale
	longueurAdresse = sizeof(struct sockaddr_in);
	memset(&pointDeRencontreLocal, 0x00, longueurAdresse);
	pointDeRencontreLocal.sin_family = PF_INET;
	pointDeRencontreLocal.sin_addr.s_addr = htonl(INADDR_ANY); // toutes les interfaces locales disponibles
	pointDeRencontreLocal.sin_port = htons(PORT);
	// On demande l’attachement local au socket
	if ((bind(socketEcoute, (struct sockaddr*)&pointDeRencontreLocal,
	longueurAdresse)) < 0)
	{
		perror("bind");
		exit(-2);
	}
	printf("Socket attaché avec succès !\n");
	// On fixe la taille de la file d’attente à 5 (pour les demandes de connexion non encore traitées)
	if (listen(socketEcoute, 5) < 0)
	{
		perror("listen");
		exit(-3);
	}
	printf("Socket placé en écoute passive ...\n");
	// boucle d’attente de connexion : en théorie, un serveur attend indéfiniment !
	while (1)
	{
		memset(messageEnvoi, 0x00, LG_MESSAGE * sizeof(char));
		memset(messageRecu, 0x00, LG_MESSAGE * sizeof(char));
		printf("Attente d’une demande de connexion (quitter avec Ctrl-C)\n\n");
		// c’est un appel bloquant
		socketDialogue = accept(socketEcoute, (struct
		sockaddr*)&pointDeRencontreDistant, &longueurAdresse);
		if (socketDialogue < 0)
		{
			perror("accept");
			close(socketDialogue);
			close(socketEcoute);
			exit(-4);
		}
		// On réceptionne les données du client
		lus = read(socketDialogue, messageRecu, LG_MESSAGE * sizeof(char));
		// ici appel bloquant
		
		// On récupère l'id
		switch (lus)
		{
			case -1: /* une erreur ! */
				perror("read");
				close(socketDialogue);
				exit(-5);
			case 0: /* socket fermé */
				fprintf(stderr, "Le socket a été fermé par le client!\n\n");
				close(socketDialogue);
			return 0;
			default: /* réception de n octets */
			printf("Message reçu : %s (%d octets)\n\n",messageRecu, lus);
		}
		
		// On envoie des données vers le client
		sprintf(messageEnvoi, "%f\n", trouverMoyenne(atoi(messageRecu),matieres));
		
		
		
		ecrits = write(socketDialogue, messageEnvoi, strlen(messageEnvoi));
		
		
		switch (ecrits)
		{
			case -1: /* une erreur ! */
				perror("write");
				close(socketDialogue);
				exit(-6);
			case 0: /*socket fermé */
				fprintf(stderr, "Le socket a été fermé par le client !\n\n");
				close(socketDialogue);
				return 0;
			default: /* envoi de n octets */
				printf("Message %s envoyé (%d octets)\n\n", messageEnvoi,ecrits);
		}
		// On ferme le socket de dialogue et on se replace en attente ...
		close(socketDialogue);
	}
	// On ferme la ressource avant de quitter
	close(socketEcoute);
	return 0;
}
