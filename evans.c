#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>


int main(){
	int clients[10] , i , found, tempclients[10];
	int welcomeSocket, newSocket , clientcount , currentclient;
	char buffer[1024] , clientport[4] , outbuffer[1024];
	struct sockaddr_in serverAddr , cli_addr;
	struct sockaddr_storage serverStorage;
	socklen_t clilen;
	socklen_t addr_size;
	clientcount = 0 ;
	found = 0 ;

	//create server socket and configure
	welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);

	memset(serverAddr.sin_zero, '0', sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(5504);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//bind to socket
	bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

	//listen
	if(listen(welcomeSocket,5)==0)
	printf("Server is listening \n");
	else
	printf("Error\n");


	//accept connections
	addr_size = sizeof serverStorage;
	cli_addr.sin_family = AF_INET ;
	clilen = sizeof(cli_addr);
	
	while(1){
	newSocket = accept(welcomeSocket, (struct sockaddr *) &cli_addr, &clilen);
		//diplay the client port
		printf("\n new client on port: %d" , cli_addr.sin_port 	);

		for( i = 0 ; i <= clientcount ; i++){
			if(clients[i] == cli_addr.sin_port ){
				found = 1 ;
			}
		}
		if(found == 0){
			clients[clientcount] = cli_addr.sin_port ;
		}
		bzero(outbuffer , 1024);
		strcpy(outbuffer, "");
		tempclients[0] = cli_addr.sin_port ;
		sprintf(clientport , "%d" , tempclients[0] );
		strcat(outbuffer, clientport);
		strcat(outbuffer, ",");

		for( i = 1 ; i <= clientcount ; i++){
			tempclients[i] = clients[i-1];
			sprintf(clientport , "%d" , tempclients[i]  );
			strcat(outbuffer, clientport);
			strcat(outbuffer, ",");
			printf("\nclient: %d" , clients[i] );
		}

		clientcount++ ;
		printf("\nBuffer: %s" , outbuffer );
		send(newSocket,outbuffer,strlen(outbuffer),0);
  }

  return 0;
}
