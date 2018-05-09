#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024		//BUFFER SIZE 
#define PORT 2001				//PORT NUMBER

int main(int argc, char *argv[]) {

	int SocketFD;							//SOCKET COMMUNICATION
	int RecieveFlags = 0;					//0, it means that no flags are specified
	char on = 1;
	
	struct sockaddr_in server;	  			//SERVER IP 
    struct sockaddr_in client;	   			//CLIENT IP

	char server_addr[16];					//IP ADDRESS
	sprintf(server_addr, "%s", argv[1]);	//GET IP ADDRESS FROM ARGV
	int ip = inet_addr(server_addr);
	char buffer[BUFFER_SIZE + 1];			//MESSAGE STRING

	int bytes;								//MESSAGE SIZE
	int rcvsize;							//RECIEVED BYTES
	int trnmsize;							//TRANSMITED BYTES
	int serverSize = sizeof server;
	int clientSize = sizeof client;

	server.sin_family = AF_INET;
    server.sin_addr.s_addr = ip;
    server.sin_port = htons(PORT);

    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    
  	setsockopt(SocketFD, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
    setsockopt(SocketFD, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

    /* CONNECT TO STERVER */
   	connect(SocketFD, (struct sockaddr *) &server, serverSize);
   
   	RE:
   	bytes = 1024;

   	//waiting for server message 
	rcvsize = recv(SocketFD, buffer, bytes, RecieveFlags);  

    printf("\n%s\n", buffer); 	

    //check the game status 
    if(buffer[0] == 'V') {

    	close(SocketFD);
    	return 0;
    	
    } else {

    	//clear buffer 
    	memset(buffer, 0, BUFFER_SIZE + 1);

        printf(" Kersz lapot(i)/Megalsz(m)/Ujra(u) ?");
        scanf("%s",buffer);
   	    bytes = strlen(buffer)+1;

   	    //send command to server
        trnmsize = send(SocketFD, buffer, bytes, RecieveFlags);

        if(buffer[0] == 'i' || buffer[0] == 'u') { 
    	    memset(buffer, 0, BUFFER_SIZE + 1);
       	    goto RE;
        }

        if(buffer[0] == 'm') {

    	    memset(buffer, 0, BUFFER_SIZE + 1);
    	    bytes = 1024;
    	    rcvsize = recv(SocketFD, buffer, bytes, RecieveFlags); 

    	    printf("\n%s\n", buffer);	
        }

    }

    return 0;

}