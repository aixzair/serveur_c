#ifndef H_MESSAGE_H
#define H_MESSAGE_H

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define LONGUEUR_MESSAGE 500

typedef enum Message_e {
    MESSAGE_OK = 1,
    MESSAGE_ERREUR = -1,
    MESSAGE_SOCKET = -2
} Message;

Message recevoirMessage(int socket, char *message, int longueur);
Message envoyerMessage(int socket, const char *message, int longueur);

#endif
