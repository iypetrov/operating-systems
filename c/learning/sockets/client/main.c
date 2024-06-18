#include <stdio.h>
#include <err.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 8080

int main(void) {
  // Create a socket
  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) err(1, "failed socket"); 

  // Set up server addr 
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  // Connect to the server
  if (connect(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) err(1, "failed connect");
  dprintf(1, "Connected to server\n");

// Send message to the server
  const char *msg = "Hello from client";
  if (write(sfd, msg, strlen(msg)) == -1) err(1, "failed write msg to server");
  dprintf(1, "Sent message: %s\n", msg);

  // Read server message
  char buf[1024];
  memset(buf, 0, sizeof(buf)); 
  ssize_t rs = read(sfd, buf, sizeof(buf) - 1);
  if (rs == -1) err(1, "failed to read msg from server");
  buf[rs] = '\0'; 
  dprintf(1, "Received message: %s\n", buf);

  close(sfd);
  return 0;
}
