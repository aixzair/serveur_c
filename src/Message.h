#ifndef H_MESSAGE_H
#define H_MESSAGE_H

#define LONGUEUR_MESSAGE 500

short recevoirMessage(int socket, char *message, int longueur);
short envoyerMessage(int socket, char *message, int longueur);

#endif
