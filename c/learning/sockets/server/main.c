#include <stdio.h>
#include <err.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 8080
#define BACKLOG 5

int main(void) {
  // Create a socket
  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) err(1, "failed socket"); 

  // Bind socket to addr
  struct sockaddr_in my_addr;
  memset(&my_addr, 0, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_addr.s_addr = INADDR_ANY;
  my_addr.sin_port = htons(PORT);
  if (bind(sfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1) err(1, "failed bind");

  // Listen for incoming connections
  if (listen(sfd, BACKLOG) == -1) err(1, "failed listen");
  dprintf(1, "Server is listening on port %d\n", PORT);

  // Accept incoming connection
  struct sockaddr_in peer_addr;
  socklen_t addr_len = sizeof(struct sockaddr_in);
  int cfd = accept(sfd, (struct sockaddr *)&peer_addr, &addr_len);
  if (cfd == -1) err(1, "failed accept");
  dprintf(1, "Connected to client\n");

  // Read client message
  char buf[1024];
  memset(buf, 0, sizeof(buf)); 
  ssize_t rs = read(cfd, buf, sizeof(buf) - 1);
  if (rs == -1) err(1, "failed read msg from client");
  buf[rs] = '\0'; 
  dprintf(1, "Received message: %s\n", buf);

  // Send message to client
  const char *resp = "Hello from server";
  if (write(cfd, resp, strlen(resp)) == -1) err(1, "failed to write msg to client");
  dprintf(1, "Sent message: %s\n", resp);

  close(cfd);
  close(sfd);
  return 0;
}
