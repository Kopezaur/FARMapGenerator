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
#include <assert.h>
#include "MapServer.h"

#define PORT 5555

// ----- Variables globales

Map maps[10];
int size;

// ----- Function to generate a random integer between [0, n)

/* Returns an integer in the range [0, n).
 *
 * Uses rand(), and so is affected-by/affects the same seed.
 */
int randint(int n) {
      if ((n - 1) == RAND_MAX) {
        return rand();
      } else {
        // Supporting larger values for n would requires an even more
        // elaborate implementation that combines multiple calls to rand()
        assert (n <= RAND_MAX);

        // Chop off all of the values that would cause skew...
        int end = RAND_MAX / n; // truncate skew
        assert (end > 0);
        end *= n;

        // ... and ignore results from rand() that fall above that limit.
        // (Worst case the loop condition should succeed 50% of the time,
        // so we can expect to bail out of this loop pretty quickly.)
        int r;
        while ((r = rand()) >= end);

        return r % n;
      }
}

// ----- Function which generates a randomly seeded map

struct Map generateMap(void)
{
    // members initialization
    srand ( time(NULL) );
    Map m;
    size = randint(10) + 10;
    printf("Size = %d\n", size);
    int walls = randint((size*size)/4);
    printf("Walls = %d\n", walls);
    int water = randint((size*size)/10);
    printf("Water = %d\n", water);
    int x, y;
    Position mercs[6];
    Position thebes;
    Position oedipe;
    Position sphinx;
    int cond = 1;

    while(cond){
        // matrix initialization
        for(int i=0;i<size;i++) {
            for(int j=0;j<size;j++) {
                m.matrix[i][j] = 0;
            }
        }

        // placement of the default elements
        //walls
        for(int i=0;i<walls;i++) {
            x = randint(size);
            y = randint(size);
            while(m.matrix[x][y]!=0){
                x = randint(size);
                y = randint(size);
            }
            m.matrix[x][y] = 2;
        }
		
        //water
        for(int i=0;i<water;i++) {
            x = randint(size);
            y = randint(size);
            while(m.matrix[x][y]!=0){
                x = randint(size);
                y = randint(size);
            }
            m.matrix[x][y] = 1;
        }
		
        //mercenaries
        for(int i=0;i<6;i++) {
            x = randint(size);
            y = randint(size);
            while(m.matrix[x][y]!=0){
                x = randint(size);
                y = randint(size);
            }
            m.matrix[x][y] = 3;
        }
		
        //thebes
        x = randint(size);
        y = randint(size);
        while(m.matrix[x][y]!=0){
            x = randint(size);
            y = randint(size);
        }
        m.matrix[x][y] = 4;
		
        //oedipe
        x = randint(size);
        y = randint(size);
        while(m.matrix[x][y]!=0){
            x = randint(size);
			y = randint(size);
        }
        m.matrix[x][y] = 5;
		
        //sphinx
        x = randint(size);
        y = randint(size);
        while(m.matrix[x][y]!=0){
            x = randint(size);
            y = randint(size);
        }
        m.matrix[x][y] = 6;
	
		// test print to show all the positions on the generated map
        printf("The test matrix :\n");
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                printf("%d, ", m.matrix[i][j]);
            }
            printf("\n");
        }

        // Assume that the map is valid
        int noOfMercs = 0;
        for(int i=0;i<size;i++) {
            for(int j=0;j<size;j++) {
                if(m.matrix[i][j] == 3){ // if it's a mercenary
                    m.matrix[i][j] = 0;
                    mercs[noOfMercs].x = i;
                    mercs[noOfMercs].y = j;
                    noOfMercs++;
                } else if(m.matrix[i][j] == 4) { // if it's thebes
                    m.matrix[i][j] = 0;
                    thebes.x = i;
                    thebes.y = j;
                } else if(m.matrix[i][j] == 5) { // if it's oedipe
                    m.matrix[i][j] = 0;
                    oedipe.x = i;
                    oedipe.y = j;
                } else if(m.matrix[i][j] == 6) { // if it's the sphinx
                    m.matrix[i][j] = 0;
                    sphinx.x = i;
                    sphinx.y = j;
                } 
            }
        }

        // create the auxiliary structure
        ObjectivePosition positions;
        positions.nbVillager  = randint(10) + 1;
        positions.mercenaries[0]= mercs[0];
        positions.mercenaries[1]= mercs[1];
        positions.mercenaries[2]= mercs[2];
        positions.mercenaries[3]= mercs[3];
        positions.mercenaries[4]= mercs[4];
        positions.mercenaries[5]= mercs[5];
        positions.thebes        = thebes;
        positions.oedipe        = oedipe;
        positions.sphinx        = sphinx;
        // create the main structure of the map
        m.objPos = positions;
		m.mapSize = size; 
        cond = 0;
    }
    return m;
}

// ----- Function which generates the default maps

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

    for(int i = 0; i<15; i++){
        for(int j = 0; j < 15; j++){
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
	map1.mapSize = 15;
    maps[0] = map1;
}

// ----- Main fonction

int main(void) {

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

		/*pthread_create(&thr, NULL, thread, (void *) &clientSocket);
		pthread_detach(thr);*/

		char buffer[2048];

		// R�ception de la requ�te
		recv(clientSocket, buffer, sizeof(buffer), 0);

		// Pour savoir quel type de carte est demand�
		char *token = strtok(buffer, " ");

		if (strcmp(token, "default") == 0) {

			//int r = rand() % 6;

			if(sendto(clientSocket, &maps[0], sizeof(maps[0]), 0, (struct sockaddr*) &csin, sizeof(csin)) < 0) {
				printf("ERROR : envoi de la carte\n");
			} else {
				printf("SUCCESS : carte envoyee\n");
			}

		} else if (strcmp(token, "random") == 0) {
			
			Map map = generateMap();

			if(sendto(clientSocket, (void*)&map, sizeof(map), 0, (struct sockaddr*) &csin, sizeof(csin)) < 0) {
				printf("ERROR : envoi de la carte\n");
			} else {
				printf("SUCCESS : carte envoyee\n");
			}

		} else {
			char buffError[128] = "Requete incorrecte !";
			printf("%s\n", buffError);
			send(clientSocket, buffError, 128, 0);
		}
	}

	/* Fermeture de la socket serveur et client*/
	close(clientSocket);
	close(serverSocket);

	return EXIT_SUCCESS;
}