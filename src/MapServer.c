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
#include "MapServer.h"

#define PORT 7777

// ----- Variables globales

Map maps[10];

// ----- 

void defaultMapsGeneration(void) {
	/*Position m1 = {9, 1};
	Position m2 = {9, 2};
	Position m3 = {9, 3};
	Position m4 = {9, 7};
	Position m5 = {9, 8};
	Position m6 = {9, 9};
	Position mercenaries1[6] = {m1, m2, m3, m4, m5, m6};
	Position thebes1 = {10, 5};
	Position oedipe1 = {6, 5};
	Position sphinx1 = {0, 5};
	ObjectivePosition op1 = { nbVillager = 5, mercenaires = mercenaries1, thebes = thebes1, oedipe = oedipe1, sphinx = sphinx1 };
	int matrix1[10][10] = { { 0, 0, 0, 0, 0, 0, 2, 2, 2, 2 }, 
							{ 0, 2, 0, 0, 0, 0, 0, 0, 0, 0 }, 
							{ 2, 2, 0, 0, 0, 0, 0, 0, 0, 0 },
							{ 0, 2, 0, 0, 1, 0, 1, 0, 0, 0 },
							{ 0, 2, 0, 0, 1, 0, 1, 0, 0, 0 },
							{ 0, 0, 0, 0, 1, 1, 1, 0, 0, 0 },
							{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
							{ 0, 0, 0, 2, 2, 2, 2, 2, 0, 0 },
							{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
							{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
	Map map1 = { matrix = matrix1, objPos = op1 };
	maps[0] = map1;*/

	ObjectivePosition positions;
    Map map1;

    int mapGrid[15][15] = {
            { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
            { 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
            { 2, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
            { 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2 },
            { 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 0, 1, 1, 0, 2 },
            { 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2 },
            { 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 2, 2, 0, 0, 2 },
            { 2, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 2, 0, 2 },
            { 2, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 2, 0, 2 },
            { 2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 2, 2, 0, 2 },
            { 2, 0, 0, 2, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2 },
            { 2, 0, 0, 2, 0, 0, 0, 0, 1, 1, 0, 2, 2, 0, 2 },
            { 2, 0, 0, 2, 0, 0, 0, 0, 1, 1, 0, 2, 2, 0, 2 },
            { 2, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 2 },
            { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 }
        };

    int i;
    int j;
    for(i = 0; i<15; i++){
        for(j = 0; j < 15; j++){
            map1.matrix[i][j] = mapGrid[i][j];
        }
    }

    Position mercenaire1;
    mercenaire1.x = 1;
    mercenaire1.y = 1;

    Position mercenaire2;
    mercenaire2.x = 13;
    mercenaire2.y = 1;

    Position mercenaire3;
    mercenaire3.x = 1;
    mercenaire3.y = 13;

    Position mercenaire4;
    mercenaire4.x = 5;
    mercenaire4.y = 5;

    Position mercenaire5;
    mercenaire5.x = 13;
    mercenaire5.y = 7;

    Position mercenaire6;
    mercenaire6.x = 1;
    mercenaire6.y = 8;

    Position thebes;
    thebes.x = 5;
    thebes.y = 7;

    Position oedipe;
    oedipe.x = 9;
    oedipe.y = 4;

    Position sphinx;
    sphinx.x = 2;
    sphinx.y = 13;

    positions.nbVillager  = 5;
    positions.mercenaries[0]= mercenaire1;
    positions.mercenaries[1]= mercenaire2;
    positions.mercenaries[2]= mercenaire3;
    positions.mercenaries[3]= mercenaire4;
    positions.mercenaries[4]= mercenaire5;
    positions.mercenaries[5]= mercenaire6;
    positions.thebes        = thebes;
    positions.oedipe        = oedipe;
    positions.sphinx        = sphinx;

    map1.objPos = positions;
    maps[0] = map1;
}

void * thread(void * th) {

	int csock = *(int*) th;
	char buffer[256];

	// R�ception de la requ�te
	recv(csock, buffer, sizeof(buffer), 0);

	// Pour savoir quel type de carte est demand�
	char *token = strtok(buffer, " ");

	if (strcmp(token, "default") == 0) {

		int r = rand() % 6;

		if(send(csock, (void*)&maps[r], sizeof(maps[r]), 0) < 0) {
			printf("ERROR : envoi de la carte");
		} else {
			printf("SUCCESS : carte envoyee");
		}

	} else if (strcmp(token, "random") == 0) {
		
		//MapGenerator map; // APPELER LA FONCTION
		Map map;

		if(send(csock, (void*)&map, sizeof(map), 0) < 0) {
			printf("ERROR : envoi de la carte");
		} else {
			printf("SUCCESS : carte envoyee");
		}

	} else {
		char buffError[128] = "Requete incorrecte !";
		printf("%s\n", buffError);
		send(csock, buffError, 128, 0);
	}

	pthread_exit(0);
	close(csock);
}

// ----- Main fonction

int main(void) {

	pthread_t thr;

	/* Generation of the default maps */
	defaultMapsGeneration();

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
		printf("Un client est connecte avec la socket %d de %s:%d\n", clientSocket, inet_ntoa(csin.sin_addr), htons(csin.sin_port));

		pthread_create(&thr, NULL, thread, (void *) &clientSocket);
		pthread_detach(thr);
	}

	/* Fermeture de la socket serveur */
	close(serverSocket);

	return EXIT_SUCCESS;
}