#include "Message.h"

Message recevoirMessage(int socket, char *message, int longueur) {
	switch (read(socket, message, LONGUEUR_MESSAGE * sizeof(char))) {
	case -1:
		return MESSAGE_ERREUR;
	case 0:
		return MESSAGE_SOCKET;
	}
	return MESSAGE_OK;
}

Message envoyerMessage(int socket, const char *message, int longueur) {
	switch (write(socket, message, longueur)) {
	case -1:
		return MESSAGE_ERREUR;
	case 0:
		return MESSAGE_SOCKET;
	}
	return MESSAGE_OK;
}
