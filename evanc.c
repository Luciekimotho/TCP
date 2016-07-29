#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>


int peers[10] , peercount  ;
void *StartListening(void *port)
{
	int clients[10], myport;
	int welcomeSocket, newSocket ;
	char buffer[1024] , peerport[4];
	struct sockaddr_in serverAddr , cli_addr;
	struct sockaddr_storage serverStorage;
	socklen_t clilen;
	socklen_t addr_size;


	myport = (int) port ;
	//create server socket and configure
	welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);
	memset(serverAddr.sin_zero, '0', sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(myport);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");


	//bind to socket
	bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

	//listen
	if(listen(welcomeSocket,5)==0)
	printf("\npeer is listening \n");
	else
	printf("Error\n");

	//accept connections
	addr_size = sizeof serverStorage;
	cli_addr.sin_family = AF_INET ;
	clilen = sizeof(cli_addr);
	while(1){
	newSocket = accept(welcomeSocket, (struct sockaddr *) &cli_addr, &clilen);
		//diplay the client port
		//printf("\n peer on port: %d" , cli_addr.sin_port 	);

		//read message
		recv(newSocket, buffer, 1024, 0);
		printf("\nmessage received: %s" , buffer);

		//reply with client port
		strcpy(buffer, "acknowledged");
		send(newSocket,buffer,strlen(buffer),0);



  }

	return NULL ;
	}


void SendMessage(void *port ){
	int n  , senderport ;
	int clientSocket;
	char buffer[1024];
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;

	senderport = (int) port ;
	//client socket
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	//set server values
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(senderport);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	//connect to server
	addr_size = sizeof serverAddr;
	connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

	//get a  message and send to the server
	printf("\nsending  message: ");
	bzero(buffer, 256) ;
	//fgets(buffer, 255, stdin);
	strcpy(buffer , "hello from me");

	n = write(clientSocket , buffer , strlen(buffer));

	//read response
	recv(clientSocket, buffer, 1024, 0);
	printf("\n peer response: %s",buffer);
	return NULL ;
}

int ConnectToServer(){
	int n , myport   ;
	int clientSocket;
	char buffer[1024];
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	char* token;
	char* string;
	char* tofree;

	peercount = 0 ;
	//client socket
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	//set server values
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(5504);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	//connect to server
	addr_size = sizeof serverAddr;
	connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

	//read response
	recv(clientSocket, buffer, 1024, 0);
	printf("peers: %s\n"  , buffer);

	//split the input
	token = strtok(buffer, ",");
	while(token != NULL){
		peers[peercount] = atoi(token);
		token = strtok(NULL, ",");
		peercount++ ;
		}

	bzero(buffer, 256) ;
	strcpy(buffer , "hello");

	//n = write(clientSocket , buffer , strlen(buffer));
	return peers[0];
}




int main() {
	int randomnumber, peerport , messagescount;
	pthread_t pth , pth2;
	int myport ;
	randomnumber = 0 ;
	myport = ConnectToServer();
	printf("my port number is:%d\n" , myport);

	pthread_create(&pth , NULL , StartListening , (void*)myport);

	printf("peer count %d\n " , peercount);

	//pthread_create(&pth2 , NULL , SendMessage , 5500);

	//printf("\nenter peer port");
	//scanf("%d" , &peerport);
	//SendMessage(peerport);

	if(peercount == 1 ){
		printf("there are no other peers to connect with");
	}
	else{
		while(randomnumber == 0 ){
			randomnumber = rand() % peercount;
		}
		//printf("selected peer %d\n", randomnumber);
		printf("selected peer %d\n", peers[randomnumber]);
		for(messagescount = 1 ; messagescount <=5 ; messagescount++){
			SendMessage(peers[randomnumber]);
		}
    }
	getchar();
}
