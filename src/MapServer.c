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
#include <time.h>
//#include "Map.h"
//#include "MapGenerator.h"

#define PORT 10000
Map maps[10];

void * thread(void * th) {

	int csock = *(int*) th;
	char buffer[256];

	// R�ception de la requ�te
	recv(csock, buffer, sizeof(buffer), 0);

	// Pour savoir quel type de carte est demand�
	char *token = strtok(buffer, " ");

	if (strcmp(token, "default") == 0) {

		int rand = rand_a_b(0,5);

		if(send(csock, (void*)&maps[rand], sizeof(maps[rand]), 0) < 0) {
			printf("ERROR : envoi de la carte");
		} else {
			printf("SUCCESS : carte envoy�e");
		}

	} else if (strcmp(token, "random") == 0) {
		
		MapGenerator map; // APPELER LA FONCTION

		if(send(csock, (void*)&map, sizeof(map), 0) < 0) {
			printf("ERROR : envoi de la carte");
		} else {
			printf("SUCCESS : carte envoy�e");
		}

	} else {
		char buffError[128] = "Requ�te incorrecte !";
		printf("%s\n", buffError);
		send(csock, buffError, 128, 0);
	}

	pthread_exit(0);
	close(csock);
}

int main(void) {

	pthread_t thr;

	/* Generation of the default maps */
	defaultMapGeneration();

	/* Socket et contexte d'adressage du serveur */
	struct sockaddr_in sin;
	int serverSocket;
	socklen_t recsize = sizeof(sin);

	/* Socket et contexte d'adressage du client */
	struct sockaddr_in csin;
	int clientSocket;
	socklen_t crecsize = sizeof(csin);

	/* Cr�ation d'une socket */
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	/* Configuration */
	sin.sin_addr.s_addr = htonl(INADDR_ANY); /* Adresse IP automatique */
	sin.sin_family = AF_INET; /* Protocole familial (IP) */
	sin.sin_port = htons(PORT); /* Listage du port */
	bind(serverSocket, (struct sockaddr*) &sin, recsize);

	/* D�marrage du listage */
	listen(serverSocket, 15);

	while (1) {
		/* Attente d'une connexion client */
		clientSocket = accept(serverSocket, (struct sockaddr*) &csin, &crecsize);
		printf("Un client est connect� avec la socket %d de %s:%d\n", clientSocket, inet_ntoa(csin.sin_addr), htons(csin.sin_port));

		pthread_create(&thr, NULL, thread, (void *) &clientSocket);
		pthread_detach(thr);
	}

	/* Fermeture de la socket serveur */
	close(serverSocket);

	return EXIT_SUCCESS;
}

void defaultMapsGeneration(){
	Position m1 = {9, 1};
	Position m2 = {9, 2};
	Position m3 = {9, 3};
	Position m4 = {9, 7};
	Position m5 = {9, 8};
	Position m6 = {9, 9};
	Position mercenaries1[6] = {m1, m2, m3, m4, m5, m6};
	Position thebes1 = {10 , 5};
	Position oedipe1 = {6, 5};
	Position sphinx1 = {0, 5};
	ObjectPosition op1 = {5, mercenaries1, thebes1, oedipe1, sphinx1};
	Map map1 = {{
		    {0, 0, 0, 0, 0, 0, 2, 2, 2, 2},
			{0, 2, 0, 0, 0, 0, 0, 0, 0, 0},
			{2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 2, 0, 0, 1, 0, 1, 0, 0, 0},
			{0, 2, 0, 0, 1, 0, 1, 0, 0, 0},
			{0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 2, 2, 2, 2, 2, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
			},
			op1
			};
	maps[0] = map1;
}
