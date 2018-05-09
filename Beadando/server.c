#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <time.h> 

#define BUFFER_SIZE 1024
#define DATA_SIZE 100
#define PORT 2001
#define DECKSIZE 32


/*DECK BUILD*/

struct Card {

    char Suit[8];
    char Face[8];
    int PositionValue;
    int Value;

} Deck[DECKSIZE] = {

    //Ászok
    { "Zold", "Asz", 0, 11 },
    { "Makk", "Asz", 1, 11 },
    { "Tok", "Asz", 2, 11 },
    { "Piros", "Asz", 3, 11 },

    //Királyok
    { "Zold", "Kiraly", 4, 4 },
    { "Makk", "Kiraly", 5, 4 },
    { "Tok", "Kiraly", 6, 4 },
    { "Piros", "Kiraly", 7, 4 },

    //Felsők
    { "Zold", "Felso", 8, 3 },
    { "Makk", "Felso", 9, 3 },
    { "Tok", "Felso", 10, 3 },
    { "Piros", "Felso", 11, 3 },

    //Alsók
    { "Zold", "Also", 12, 2 },
    { "Makk", "Also", 13, 2 },
    { "Tok", "Also", 14, 2 },
    { "Piros", "Also", 15, 2 },

    //Tizes
    { "Zold", "Tizes", 16, 10 },
    { "Makk", "Tizes", 17, 10 },
    { "Tok", "Tizes", 18, 10 },
    { "Piros", "Tizes", 19, 10 },

    //Kilences
    { "Zold", "Kilences", 20, 9 },
    { "Makk", "Kilences", 21, 9 },
    { "Tok", "Kilences", 22, 9 },
    { "Piros", "Kilences", 23, 9 },

    //Nyolcas
    { "Zold", "Nyolcas", 24, 8 },
    { "Makk", "Nyolcas", 25, 8 },
    { "Tok", "Nyolcas", 26, 8 },
    { "Piros", "Nyolcas", 27, 8 },

    //Hetes
    { "Zold", "Hetes", 28, 7 },
    { "Makk", "Hetes", 29, 7 },
    { "Tok", "Hetes", 30, 7 },
    { "Piros", "Hetes", 31, 7 },
};

void print_deck_DEBUG(int a[]) {

	for (int i = 0; i < DECKSIZE; i++) {

        //printf("%d, ", a[i]);
        for(int j = 0; j < DECKSIZE; j++) {

            if(a[i] == Deck[j].PositionValue) {

                printf("%s, ", Deck[j].Face);

            }

        }

	}

	printf("\n");
	return;
}

void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

void shuffle_deck(int a[]) {
	int index, i;
	int high, low, random;

	for(i=0; i<DECKSIZE; i++) {

		high = DECKSIZE-1;
		low = i;
        srand(time(NULL));
		random = rand();

		index = rand() % (high - low + 1) + low;

		swap(a+i, a+index);
	}

	return;
}

/*DECK BUILD*/

int main(int argc, char *argv[] ) {

	/* SOCKET things */

	int ClientSocketFD;			//SOCKET COMMUNICATION
	int ServerSocketFD;			//SOCKET COMMUNICATION

	int RecieveFlags = 0;		//0, it means that no flags are specified
	int bytes;					//MESSAGE SIZE
	int rcvsize;   				//RECIEVED BYTES                 	
    int trnmsize;     			//TRANSMITED BYTES              	
	
	char on = 1;
	char buffer[BUFFER_SIZE + 1];

	struct sockaddr_in server;
	struct sockaddr_in client;

	int ServerSize = sizeof server;
	int ClientSize = sizeof client;

    /* SOCKET things */

    /* 	GAME things */

	int sumClient = 0;
	int sumServer = 0;
	int counter = 0;

	int cards[DECKSIZE] = {0};

	for (int i = 0; i < DECKSIZE; i++) {
		
		cards[i] = i+1;
	}

	

    /* 	GAME things */

	server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bytes = strlen(buffer)+1;

    ServerSocketFD = socket(AF_INET, SOCK_STREAM, 0 );

    bind(ServerSocketFD, (struct sockaddr *) &server, ServerSize);
    listen(ServerSocketFD, 10);


    setsockopt(ServerSocketFD, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
    setsockopt(ServerSocketFD, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

    ClientSocketFD = accept(ServerSocketFD, (struct sockaddr *) &client, &ClientSize);

    RESTART:

    counter = 0;
    sumClient = 0;
    sumServer = 0;

    shuffle_deck(cards);

    char sendData1[DATA_SIZE];

    char sendData2[DATA_SIZE];
    char sendFullData[DATA_SIZE + DATA_SIZE]; 

    for(int i = 0; i < 32; i++) {

    	if(cards[counter] == Deck[i].PositionValue) {

    		strcpy(sendData1, "Oszto:\t");

    		strcat(sendData1, "Most: ");
    		strcat(sendData1, Deck[i].Suit);
    		strcat(sendData1, " ");
    		strcat(sendData1, Deck[i].Face);
    		strcat(sendData1, " / Ossz : - \t");
    		printf("%s\n", sendData1);
    		char buf[10];
    		sumServer += Deck[i].Value;
    		sprintf(buf, "%d", sumServer);
    		strcat(sendData1, buf);
    		
    	}

    }

    NEW_CARD:

    counter++;

    for(int i = 0; i < 32; i++) {

    	if(cards[counter] == Deck[i].PositionValue) {

    		strcpy(sendData2, "\n");

    		strcat(sendData2, "Te lapod:\t");
    		strcat(sendData2, "Most: ");
    		strcat(sendData2, Deck[i].Suit);
    		strcat(sendData2, " ");
    		strcat(sendData2, Deck[i].Face);
    		strcat(sendData2, "/ Ossz : - \t");
    		char buf[10];
    		sumClient += Deck[i].Value;
    		sprintf(buf, "%d", sumClient);
    		strcat(sendData2, buf);
    		printf("%s\n", sendData2);
    		
    	}    	

    }

    if(sumClient > 21) {

    	strcat(sendFullData, "\nVesztettel!");
    	strncpy(buffer,  sendFullData  , BUFFER_SIZE + 1);
        bytes = strlen(buffer) + 1;
        trnmsize = send(ClientSocketFD, buffer, bytes, RecieveFlags);

        goto LOSE;

    }

    strcpy(sendFullData, sendData1);
    strcat(sendFullData, sendData2);

    strncpy(buffer, sendFullData , BUFFER_SIZE + 1);


    printf("%s\n", buffer);

    bytes = strlen(buffer) + 1;
    trnmsize = send(ClientSocketFD, buffer, bytes, RecieveFlags);

    memset(sendData2, 0, DATA_SIZE);
    memset(sendFullData, 0, DATA_SIZE + DATA_SIZE);
    memset(buffer, 0, BUFFER_SIZE + 1);

    rcvsize = recv( ClientSocketFD, buffer, bytes, RecieveFlags );


    if(buffer[0] == 'i') {

    	memset(buffer, 0, BUFFER_SIZE + 1);
    	goto NEW_CARD;
    }

    if(buffer[0] == 'u') {

    	memset(buffer, 0, BUFFER_SIZE + 1);
    	goto RESTART;
    }

    if(buffer[0] == 'm') {

    	MEG:

    	for(int i = counter; i < 32; i++) {

    		if(cards[i] == Deck[i].PositionValue) {

    			sumServer += Deck[i].Value;
    			counter++;
    		}

    	}

    	if(sumServer < 18) {

    		printf("%s\n", "MEGEgyszer");
    		goto MEG;

    	} else {

    		if(sumServer > sumClient && sumServer < 22) {

    			memset(buffer, 0, BUFFER_SIZE + 1);
    			strcpy(buffer, "Vesztettel!");

    			strcat(buffer, "\t Oszto lap erteke:");
    			char buf[10];
    			sprintf(buf, "%d", sumServer);
    			strcat(buffer, buf);

    			printf("%s\n", buffer);

    			bytes = strlen(buffer) + 1;
    			trnmsize = send(ClientSocketFD, buffer, bytes, RecieveFlags);

    		}    		

    		if(sumServer == sumClient && sumServer < 22) {

    			memset(buffer, 0, BUFFER_SIZE + 1);
    			strcpy(buffer, "Dontetlen!");
    			strcat(buffer, "\t Oszto lap erteke:");
    			char buf[10];
    			sprintf(buf, "%d", sumServer);
    			strcat(buffer, buf);

    			printf("%s\n", buffer);

    			bytes = strlen(buffer) + 1;

    			trnmsize = send(ClientSocketFD, buffer, bytes, RecieveFlags);
    		}

    		else {

    			memset(buffer, 0, BUFFER_SIZE + 1);
    			strcpy(buffer, "Gyoztel!");
    			strcat(buffer, "\t Oszto lap erteke:");
    			char buf[10];
    			sprintf(buf, "%d", sumServer);
    			strcat(buffer, buf);

    			printf("%s\n", buffer);

    			bytes = strlen(buffer) + 1;

    			trnmsize = send(ClientSocketFD, buffer, bytes, RecieveFlags);
    		}

    	}

    }

    LOSE:

    close(ClientSocketFD);
    close(ServerSocketFD);

    return 0;
}