#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
//#include "Map.h"
#include "MapGenerator."

#define PORT 10000

void * thread(void * th) {

	int csock = *(int*) th;
	char buffer[256];

	// Réception de la requête
	recv(csock, buffer, sizeof(buffer), 0);

	// Pour savoir quel type de carte est demandé
	char *token = strtok(buffer, " ");

	if (strcmp(token, "default") == 0) {
		// TO DO
	} else if (strcmp(token, "random") == 0) {
		// TO DO
	} else {
		char buffError[128] = "Requête incorrecte !";
		printf("%s\n", buffError);
		send(csock, buffError, 128, 0);
	}

	pthread_exit(0);
	close(csock);
}

int main(void) {

	pthread_t thr;

	/* Socket et contexte d'adressage du serveur */
	struct sockaddr_in sin;
	int serverSocket;
	socklen_t recsize = sizeof(sin);

	/* Socket et contexte d'adressage du client */
	struct sockaddr_in csin;
	int clientSocket;
	socklen_t crecsize = sizeof(csin);

	/* Création d'une socket */
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	/* Configuration */
	sin.sin_addr.s_addr = htonl(INADDR_ANY); /* Adresse IP automatique */
	sin.sin_family = AF_INET; /* Protocole familial (IP) */
	sin.sin_port = htons(PORT); /* Listage du port */
	bind(serverSocket, (struct sockaddr*) &sin, recsize);

	/* Démarrage du listage */
	listen(serverSocket, 15);

	while (1) {
		/* Attente d'une connexion client */
		clientSock = accept(serverSock, (struct sockaddr*) &csin, &crecsize);
		printf("Un client est connecté avec la socket %d de %s:%d\n", clientSocket, inet_ntoa(csin.sin_addr), htons(csin.sin_port));

		pthread_create(&thr, NULL, thread, (void *) &csock);
		pthread_detach(thr);
	}

	/* Fermeture de la socket serveur */
	close(server_socket);

	return EXIT_SUCCESS;
}
