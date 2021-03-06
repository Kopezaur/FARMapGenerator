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

#define PORT 5555

int main(void) {
	
	char buffer[256];
	char bufferReceive[2048];

	struct sockaddr_in sin;
	int sock;
	socklen_t recsize = sizeof(sin);

	/* Cr�ation d'une socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);

	/* Configuration */
	sin.sin_addr.s_addr = htonl(INADDR_ANY); /* Adresse IP automatique */
	sin.sin_family = AF_INET; /* Protocole familial (IP) */
	sin.sin_port = htons(PORT); /* Listage du port */

	/* Demarrage du listage */
	connect(sock, (struct sockaddr*)&sin, sizeof(sin));
	printf("Un client est connecte avec la socket %d de %s:%d\n", sock, inet_ntoa(sin.sin_addr), htons(sin.sin_port));

	sprintf(buffer, "random");

	send(sock, buffer, sizeof(buffer), 0);

	Map map;

	if(recv(sock, &map, sizeof(map), 0)) {

		printf("Structure reçue\n");

		printf("%d\n", map.objPos.nbVillager);

        for(int i = 0; i < map.mapSize; i++){
	        for(int j = 0; j < map.mapSize; j++){
	            printf("%d, ", map.matrix[i][j]);
	        }
	   		printf("\n");
	    }
	} else {
		printf("Fail");
	}

	close(sock);

	return EXIT_SUCCESS;
}
