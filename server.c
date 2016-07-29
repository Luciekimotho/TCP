#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
  int socket_desc, client_sock, new_sock, address_size, read_size;
  struct sockaddr_in server, client;
  char client_msg[2000];
  socklen_t addr_size;
  struct sockaddr_storage serverStorage;

  //Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_desc == -1) {
    printf("Could not create socket\n");
  }
  printf("Socket created\n");

   //settings of the server address struct
   server.sin_family = AF_INET;     //family
   server.sin_addr.s_addr = inet_addr("127.0.0.1");   //ip address
   server.sin_port = htons(8888);   //port number


  //binding socket
  /*bind(socket_desc, (struct sockaddr *)&server, sizeof(server));*/
  if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0 ) {
    printf("Bind failed. Error.\n");
    return 1;
  }
  printf("Bind successful.\n");

  //listening
  listen(socket_desc, 5);   //max of 5 clients
  printf("Waiting for incoming connections.\n");

  //accept
  while (1) {
    /* code */
    address_size = sizeof(struct sockaddr_in);
    new_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &address_size);

    if (new_sock < 0) {
      printf("Accept failed\n");
      return 1;
    }
    printf("Connection accepted\n");
    //addr_size = sizeof serverStorage;
    //new_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &addr_size);


    //Receive a message
    while ((read_size = recv(client_sock, client_msg, 2000, 0)) > 0) {
      write(client_sock, client_msg, strlen(client_msg));
        printf("recv\n");
    }

    if (read_size == 0) {
      printf("Client disconnected\n");
    }else if (read_size == -1) {
      printf("Receive failed\n");
    }
  }

  //Send msg to socket of incoming connections
  return 0;
}
