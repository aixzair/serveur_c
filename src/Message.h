#ifndef H_MESSAGE_H
#define H_MESSAGE_H

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define LONGUEUR_MESSAGE 500

/**
 * @brief indique si le message c'est envoyé
 * 
 */
typedef enum Message_e {
    MESSAGE_OK = 1,
    MESSAGE_ERREUR = -1,
    MESSAGE_SOCKET = -2
} Message;

/**
 * @brief Recois un message
 * 
 * @param socket de dialogue
 * @param message reçus
 * @param longueur du message
 * @return Message 
 */
Message recevoirMessage(int socket, char *message, int longueur);

/**
 * @brief Envoie un message
 * 
 * @param socket de dialogue
 * @param message à envoyer
 * @param longueur du message
 * @return Message 
 */
Message envoyerMessage(int socket, const char *message, int longueur);

#endif
