#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

//#define MAXCLIENTS 10


//Structure for the client
/*struct Client{
  char name[50];
  int port;
  int clientSockDescr;
}*/

//struct Client myClient[MAXCLIENTS];

//void *thread_handler(void *);

//Initializing the server socket.
//Functions for creating socket, binding and listening
int initializeServrSkt(){

  struct sockaddr_in server;
  int server_port_no=8881;
  int socket_desc;

  //Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_desc == -1) {
    printf("Could not create socket\n");
  }
  printf("Socket created\n");

  printf("Connect on port number %d\n", server_port_no);

   //settings of the server address struct
   server.sin_family = AF_INET;     //family
   server.sin_addr.s_addr = inet_addr("127.0.0.1");   //ip address
   server.sin_port = htons(server_port_no);   //port number


  //binding socket
  /*bind(socket_desc, (struct sockaddr *)&server, sizeof(server));*/
  if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0 ) {
    printf("Bind failed. Error.\n");
    return 1;
  }
  printf("Bind successful.\n");

  //listening
  if (listen(socket_desc, 5) < 0) {
    return 1;
  }
   return socket_desc;
 }

//raceful exit code to free port
 void cleanExit() {
   exit(0);
 }

int main() {
  int socket_desc, client_sock, *new_sock, address_size;
  struct sockaddr_in server, client;
  socklen_t addr_size;
  struct sockaddr_storage serverStorage;
  pthread_t s_thread;

  int read_size, read_msg_size;
  char client_msg[1024], client_user_name[50];
  int i;
  int peercount;
  peercount = 1;


  //Calling the socket initializing function.
  int serverSocket = initializeServrSkt();

  //max of 5 clients
  printf("Waiting for incoming connections...\n");

  //accept
  address_size = sizeof(struct sockaddr_in);

  while (1){
    client_sock = accept(serverSocket, (struct sockaddr *) &client, (socklen_t *) &address_size);
    printf("Client %d connected!\n", peercount);

  if (client_sock < 0) {
    printf("Accept failed.\n");
    return 1;
  }
  printf("Connection accepted.\n");


printf("\nmessage received");
  //receive message
  if (peercount <= 10) {
	  
    read_msg_size = recv(socket, client_msg, 1024, 0);
    //write(socket, client_msg, strlen(client_msg));
    printf("Message from client %d is %s\n",peercount, client_msg);
    peercount++;
  }
    //puts(client_msg);
    if (read_size == 0) {
    //  printf("Client disconnected\n");
   }else if (read_size == -1) {
     printf("Recv failed\n");
   }
    printf("--------------------------------------------------------------------------------\n");
}
  return 0;
  //Graceful exit
   signal(SIGTERM,cleanExit);
   close(serverSocket);
}


  void *thread_handler(void *socket_desc){
    int socket = *(int*)socket_desc;
    int read_size, read_msg_size;
    char client_msg[1000], client_user_name[50];
    int i;
    int peercount;
    peercount = 0;

    //receive username from the client and print it out
     //read_size = recv(socket, client_user_name, 50, 0);
     //write(socket, client_user_name, strlen(client_user_name));
    // printf("Client %s is connected.\n", client_user_name);
      //receive message
      if (peercount <= 10) {
        read_msg_size = recv(socket, client_msg, 1000, 0);
        write(socket, client_msg, strlen(client_msg));
        printf("Message from client %d: %s\n",peercount, client_msg);
        peercount++;
      }
     //puts(client_msg);
    if (read_size == 0) {
    //  printf("Client disconnected\n");
    }else if (read_size == -1) {
      printf("Recv failed\n");
    }
    free(socket_desc);
    return 0;
  }
