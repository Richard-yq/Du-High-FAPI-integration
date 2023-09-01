/**************************************************************************
 * @brief Unit test SCTP connect. 
 *
 * @details
 *
 *     Function : client 
 *    
 *     Functionality : SCTP client send hello server message to server
 *             
 *     Reference : https://gist.github.com/zonque/7d03568eab14a2bb57cb            
 *             
 *     
 ***************************************************************************/
#include "common_def.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>

// Port number to connect to 
#define PORT 62324

int main(int argc, char *argv[]) {

     // Create socket
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
  if (sock < 0) {
    perror("socket");
    exit(1);
  }
  // Specify server address 
  struct sockaddr_in servaddr;
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servaddr.sin_port = htons(PORT);

  // Connect to server
  if (connect(sock, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
    perror("connect");
    exit(1);
  }

  // Send message
  const char *msg = "Hello P5 Server!";
  ssize_t sent = sctp_sendmsg(sock, msg, strlen(msg), NULL, 0, 0, 0, 0, 0, 0);

  if (sent < 0) {
    perror("sctp_sendmsg");
    exit(1); 
  }
  // Receive response 
  char buffer[256];
  ssize_t recvd = sctp_recvmsg(sock, buffer, sizeof(buffer), NULL, NULL, NULL, NULL);

  if (recvd < 0) {
    perror("sctp_recvmsg");
    exit(1);
  }

  // Print received data
  buffer[recvd] = '\0'; // null terminate 
  printf("Received: %s\n", buffer);

  // Close socket
  close(sock);

  return 0;
}