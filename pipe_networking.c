#include "pipe_networking.h"
//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  int from_client = 0;
  mkfifo(wkp, 0666);
  int fd = open(wkp, O_RDONLY);
  char buff[256];
  while(1) {
    if(read(fd, buff, sizeof(buff)) > 0) {
      remove(wkp);
      from_client = open(buff, O_WRONLY);
      printf("%s\n", buff);
      break;
    }
  }
  return from_client;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  printf("Server\n");
  int from_client;
  from_client = server_setup();
  write(from_client, "Message from server", 255);
  printf("Returning\n");
  if(*to_client == -1) {
    printf("%s\n", strerror(errno));
    return -1;
  }
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  printf("Client handshake\n");
  int from_server;
  int wkp_fd = open(wkp, O_WRONLY);
  char buff[256];
  snprintf(buff, sizeof(buff), "%d", getpid());
  int bytes = write(wkp_fd, buff, 255);
  printf("Client wrote %d bytes\n", bytes);

  char pid [256];
  snprintf(pid, sizeof(pid), "%d", getpid());
  mkfifo(pid, 0666);
  int pp_fd = open(pid, O_RDONLY);
  char pp_buff[256];
  while(1) {
    if(read(pp_fd, pp_buff, sizeof(pp_buff)) > 0) {
      printf("Recieved message on pp from server: %s\n", pp_buff);
    }
  }
  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  int to_client  = 0;
  return to_client;
}
