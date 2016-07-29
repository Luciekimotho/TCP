#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


int main() {
  int sock;
  struct sockaddr_in server;
  char message[2000], server_reply;

  //Create socket.
  sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock == -1) {
    printf("Could not create socket\n");
  }
  printf("Socket created\n");

   //settings of the server address struct
   server.sin_family = AF_INET;     //family
   server.sin_addr.s_addr = inet_addr("127.0.0.1");   //ip address
   server.sin_port = htons(8880);   //port number

  //Connect to remote server
  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
    printf("Connect failed. Error\n");
    return 1;
  }
  printf("Connected.\n");

  while (1) {
    printf("Enter msg: \n");
    scanf("%s\n",message );

    if (send(sock, message, strlen(message), 0) < 0) {
      printf("Send failed.\n");
      return 1;
    }

    printf("Server reply: \n", server_reply);
  }
  close(sock);
  return 0;
}
