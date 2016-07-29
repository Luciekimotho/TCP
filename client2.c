#include <stdio.h>
#include <string.h>
#include <sys/socket.h> /*for the socket functions*/
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>   /*for close*/
#include <signal.h>


//global variables.
int sock;    /*client socket description*/
char client_name[50], server_reply[2000];
char myMessage[1000];
int choice;
int peers[10], peercount;

int ConnectToServer() {
  int client_port_no;
  struct sockaddr_in server;

  //Create socket.
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    printf("Could not create socket\n");
  }
  printf("Socket created\n");

  printf("Enter port number: ");
  scanf("%d", &client_port_no);

  printf("Port number entered\n");
  printf("%d\n",client_port_no);

  //settings of the server address struct
  server.sin_family = AF_INET;     //family
  server.sin_addr.s_addr = inet_addr("127.0.0.1");   //ip address
  server.sin_port = htons(client_port_no);   //port number


  //Connect to remote server
  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
    printf("Connect failed. Error\n");
    return 1;
  }
  return peers[0];
}

void SendNewMessage() {
  int client_port_no;
  struct sockaddr_in server;
  //Create socket.
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    printf("Could not create socket\n");
  }
  printf("Socket created\n");

  printf("Enter port number: ");
  scanf("%d", &client_port_no);

  printf("Port number entered\n");
  printf("%d\n",client_port_no);
  //settings of the server address struct
  server.sin_family = AF_INET;     //family
  server.sin_addr.s_addr = inet_addr("127.0.0.1");   //ip address
  server.sin_port = htons(client_port_no);   //port number
  //Connect to remote server
  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
    printf("Connect failed. Error\n");
    return 1;
  }
  return peers[0];
}

void *StartListening(void *port){
  int clients[5], clientPort;
  int welcomeSkt, newSkt;
  char buffer[1024], peerPorts[4];
  struct sockaddr_in serverAddr, clientAddr;
  struct sockaddr_storage serverStorage;
	socklen_t clilen;
	socklen_t addr_size;

  clientPort = (int)port;
  welcomeSkt = socket(AF_INET, SOCK_STREAM, 0);

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(clientPort);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  //bind
  bind(welcomeSkt, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  //listen
  if (listen(welcomeSkt, 5) == 0) {
    printf("Peer is listening\n");
  }else{
    printf("Error in peer listening\n");
  }

  //accept connections
  addr_size = sizeof serverStorage;
  clientAddr.sin_family = AF_INET ;
	clilen = sizeof(clientAddr);
	while(1){
	newSkt = accept(welcomeSkt, (struct sockaddr *) &clientAddr, &clilen);
		//read message
		recv(newSkt, buffer, 1024, 0);
		printf("\nmessage received: %s" , buffer);

		//reply with client port
		strcpy(buffer, "acknowledged");
		send(newSkt,buffer, strlen(buffer), 0);
  }
  return NULL;
}

//send a message
void sendMsg(int port){
  char buffer[1024];
  int n;

  //printf("Chat session live. \n");
  //printf("Enter message: \n");
  //scanf("%s",myMessage );
  printf("%d\n",sock );
  printf("Sending  message....");
  bzero(buffer, 256) ;

  strcpy(buffer, "lol");
  n = write(sock, buffer, strlen(buffer));

  recv(sock, buffer, 1024, 0);
  printf("Peer response is: %s\n",buffer);

  if (send(sock, myMessage, strlen(myMessage), 0) < 0) {
    printf("Send failed.\n");
    return 1;
  }
  printf("Message has been sent to peers\n");
  return NULL;
}

//function to receive message
void receiveMsg(int sock){
  int read_size;
  char client_msg[100];

  if (recv(sock, server_reply, 2000, 0) < 0 ) {
    printf("Recv failed\n");
  }
  printf("List of messsages is: \n");
  printf("-----------------------------------------\n");
  printf("%s\n",server_reply );
}

//list of online clients
void onlineClientList(){
  printf("List of online clients\n");
  printf("%s\n",client_name);
}

void mainMenu(){
  printf("Connected.\n");
  printf("Menu\n");
  printf("2. Send a message\n");
  printf("3. Show list of messages sent. \n");
  scanf("%d", &choice);


   if (choice == 2) {
    //sending message to the other clients peer to peer
    printf("\n");
    printf("--------------------------------------------------------------------------------\n");
    //sendMsg(sock);
    SendNewMessage();
    printf("\n");
  }else if (choice == 3) {
    //receiving a message from other clients
    printf("\n");
    printf("--------------------------------------------------------------------------------\n");
    receiveMsg(sock);
    printf("\n");
  }
}

int main() {
  int myport;
  pthread_t myThread;

  //connection to the server.
  myport = ConnectToServer();

  // StartListening(myport);
  pthread_create(&myThread, NULL, StartListening, (void*) myport);

  //printf("peer count %d\n " , peercount);
  //sendMsg(myport);

  printf("\n");
  printf("\n");
  printf("--------------------------------------------------------------------------------\n");
  mainMenu();
  printf("--------------------------------------------------------------------------------\n");

  //showing list of online clients
  //onlineClientList();
  //printf("\n");
  close(sock);

  return 0;
}
