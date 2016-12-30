/* 
 * Client code. See 
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
 * @param[in] server_inet_addr The string name of the server's IP address (X.X.X.X)
 * @param[in] port The port to bind the socket to.
 * @return The error code. 0 for success, error otherwise.
 */
int setup_wsa(WSADATA* wsa, SOCKET* sock, char const* server_inet_addr, int port) {
  struct sockaddr_in server_addr; /* The server address */
  int connect_err; /* The error received on connecting the socket */

  /* Initialise winsock */
  /* MAKEWORD(2, 2) means to use winsock 2.2 */
  /* &wsa will put the details of the winsock implementation in wsa. */
  printf("Initialising winsock...\n");
  if (WSAStartup(MAKEWORD(2, 2), wsa) != 0) {
    return log_wsa_err();
  }

  /* Create socket */
  /* AF_INET makes an IPv4 socket (not IPv6) */
  /* SOCK_STREAM makes a connection based protocol (like TCP) */
  /* IPPROTO_TCP specifies we use TCP */
  *sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  printf("Creating socket...\n");
  if (*sock == INVALID_SOCKET) {
    return log_wsa_err();
  }

  /* Set server address */

  /* Set address to be localhost, see documentation for details */
  InetPton(AF_INET, "127.0.0.1", &(server_addr.sin_addr.s_addr));
  server_addr.sin_family = AF_INET;   /* IPv4 */
  server_addr.sin_port = htons(5000); /* Port 5000 */

  /* Connect to server */
  printf("Connecting to server...\n");
  connect_err = connect(*sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (connect_err < 0) {
    return log_wsa_err();
  }

  return 0;
}

/**
 * A blocking functions which will wait until the server has sent a message,
 * then print that message.
 * @param[in] sock The socket to accept connections on.
 * @return The error code. 0 for success, error otherwise.
 */
int receive_msg(SOCKET* sock) {
  char server_msg[1024]; /* A 1KB buffer for the server message */
  int recv_size; /* The size of the server message */
  recv_size = recv(*sock, server_msg, sizeof(server_msg), 0);
  if (recv_size == SOCKET_ERROR) {
    return log_wsa_err();
  }
  /* Add a NUL char at the end to indicate the end of the string. printf relies
   * on this to know the length of the string - without this, we can get buffer
   * overflow errors. */
  server_msg[recv_size] = 0; 
  printf("Server replied! \n%s\n", server_msg);

  return 0;
}

#define SERVER_PORT 5000
#define SERVER_INET_ADDR "127.0.0.1"

int main(int argc, char** argv) {
  WSADATA wsa; /* Will contain winsock API details */
  SOCKET sock; /* Will contain socket details */
  int err; /* setup_wsa() err code */

  /* Setup winsock api. SERVER_INET_ADDR and SERVER_PORT are macros. */
  err = setup_wsa(&wsa, &sock, SERVER_INET_ADDR, SERVER_PORT); 
  if (err) { 
    return err;
  }
  printf("Connected to server.\n");

  receive_msg(&sock);
  return 0;
}

