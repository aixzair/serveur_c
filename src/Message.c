#include "Message.h"

short recevoirMessage(int socket, char *message, int longueur) {
	switch (read(socket, message, LONGUEUR_MESSAGE * sizeof(char))) {
	case -1:
		perror("Erreur lors de la réception des données (read)\n");
		close(socket);
		return -1;
	case 0:
		perror("Le socket a été fermé!\n");
		close(socket);
		return 0;
	}
	return 1;
}

short recevoirMessage(int socket, char *message, int longueur) {
	switch (write(socket, message, longueur)) {
	case -1:
		perror("Erreur lors de l'envoie des données (write)\n");
		return -1;
	case 0:
		perror("Le socket a été fermé!\n");
		return 0;
	}
	return 1;
}