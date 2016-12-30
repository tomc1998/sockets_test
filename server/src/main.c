/* 
 * Server code. See 
 * http://www.binarytides.com/winsock-socket-programming-tutorial/ 
 * for details 
 */

#include <stdio.h>
#include <winsock2.h>
#include <Ws2tcpip.h>

/**
 * Log winsock error, then return the error code as an integer.
 * @return The result of WSAGetLastError().
 */
int log_wsa_err() {
  int err_num = WSAGetLastError();
  printf("Failed. Error code %d", err_num);
  return err_num;
}

/**
 * Setup WSA, create a socket and bind it to the given port.
 * @param[out] wsa Winsock API details will be put here.
 * @param[out] sock Socket will be created as this address.
 * @param[in] port The port to bind the socket to.
 * @return The error code. 0 for success, error otherwise.
 */
int setup_wsa(WSADATA* wsa, SOCKET* sock, int port) {
  struct sockaddr_in server_addr; /* The server address */
  int bind_err; /* The error received on binding the socket */

  /* Do all initialisation. See client code for some more comments in regards to this. */
  printf("Initialising winsock...\n");
  if (WSAStartup(MAKEWORD(2, 2), wsa) != 0) {
    return log_wsa_err();
  }
  printf("Creating socket...\n");
  *sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (*sock == INVALID_SOCKET) {
    return log_wsa_err();
  }

  /* Set address to be INADDR_ANY (we are accepting connections, so it can be any address) */
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_family = AF_INET;   /* IPv4 */
  server_addr.sin_port = htons(port); /* Port 5000 */

  /* We need to bind the socket to a port. This means that if any connections
   * are received on this port, they will be passed to the socket. */
  bind_err = bind(*sock, (struct sockaddr*) &server_addr, sizeof(server_addr));
  if (bind_err == SOCKET_ERROR) {
    return log_wsa_err();
  }

  /* Listen for connections. Max backlog of 50. */
  listen(*sock, 50);

  return 0;
}

/**
 * A blocking function which will wait until a client has connected to the
 * socket, accept the connection, send a message and close the connection.
 * @param[in] sock The socket to accept connections on.
 * @return The error code. 0 for success, error otherwise.
 */
int accept_connection(SOCKET* sock) {
  struct sockaddr_in client_addr; /* The client addr */
  int sock_size = sizeof(struct sockaddr_in); /* The size of the client addr struct */
  SOCKET client_sock; /* The socket of the client being accepted */
  /* Message to send to client upon connected */
  const char* message = 
    "Hello Client , I have received your connection. But I have to go now, \
bye.\n";

  /* When accepting a client, we create a new socket to represent their
   * connection to our socket. */
  client_sock = accept(*sock, (struct sockaddr*)&client_addr, &sock_size);
  if (client_sock == INVALID_SOCKET) {
    return log_wsa_err();
  }
  printf("Client connected!\n");

  //Reply to client
  send(client_sock, message, strlen(message), 0);

  return 0;
}

#define SERVER_PORT 5000

int main(int argc, char** argv) {
  WSADATA wsa; /* Will contain winsock API details */
  SOCKET sock; /* Will contain socket details */
  int err; /* setup_wsa() and accept_connection() err code */

  /* Setup winsock stuff and create a socket */
  err = setup_wsa(&wsa, &sock, SERVER_PORT); 
  if (err) { 
    return err;
  }
  printf("Server socket bound to port %d\n", SERVER_PORT);

  /* Start accepting connections to sock */
  printf("Waiting for connections...\n");
  while(1) {
    err = accept_connection(&sock);
    if (err) {
      return err;
    }
  }
  return 0;
}

