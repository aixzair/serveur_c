#ifndef H_MESSAGE_H
#define H_MESSAGE_H

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define LONGUEUR_MESSAGE 500

short recevoirMessage(int socket, char *message, int longueur);
short envoyerMessage(int socket, const char *message, int longueur);

#endif
