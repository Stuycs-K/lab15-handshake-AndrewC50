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
  mkfifo(WKP, 0666);
  printf("Waiting for connection\n");
  int rd_fd = open(WKP, O_RDONLY);
  if(rd_fd == -1) printf("%s\n", strerror(errno));
  remove(WKP);
  return rd_fd;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  char cl_pid[256];
  int rd_fd, wr_fd;

  rd_fd = server_setup();
  if(rd_fd == -1) {
    printf("Init read%s\n", strerror(errno));
    return -1;
  }

  printf("Reading from WKP\n");
  if(read(rd_fd, cl_pid, sizeof(cl_pid)) < 0) {
    printf("Reading %s\n", strerror(errno));
    return -1;
  }

  printf("Opening PP %s\n", cl_pid);
  wr_fd = open(cl_pid, O_WRONLY);
  if(wr_fd == -1) {
    printf("Init write%s\n", strerror(errno));
    return -1;
  }

  char synack[256];
  snprintf(synack, sizeof(synack), "%d", rand());
  printf("Writing synack %s to client\n", synack);
  if(write(wr_fd, synack, strlen(synack)) == -1) {
    printf("Writing %s\n", strerror(errno));
    return -1;
  }

  char ack[256];
  if (read(rd_fd, ack, sizeof(ack)) < 0) {
    printf("%s\n", strerror(errno));
    return -1;
  }
  printf("Server received ack %s from client\n", ack);

  char expected[256];
  strcpy(expected, synack);
  strcat(expected, "1");
  if (strcmp(ack, expected) == 0) {
    printf("Handshake Complete, ack was: %s\n", synack);
  } else {
    printf("Error in handshake from client to server\n");
  }

  *to_client = wr_fd;
  return rd_fd;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char pid [256];
  snprintf(pid, sizeof(pid), "%d", getpid());
  mkfifo(pid, 0666);
  int wr_fd, rd_fd;
  wr_fd = open(WKP, O_WRONLY);
  printf("Opening WKP\n");
  if(wr_fd == -1) {
    printf("Open writing %s\n", strerror(errno));
    return -1;
  }

  if(write(wr_fd, pid, 255) == -1) {
    printf("Writing pid %s\n", strerror(errno));
    return -1;
  }
  printf("Client wrote pid to server %s\n", pid);

  printf("Opening PP %s\n", pid);
  rd_fd = open(pid, O_RDONLY);
  if(rd_fd == -1) {
    printf("Open reading%s\n", strerror(errno));
    return -1;
  }
  printf("Removing PP\n");
  remove(pid);

  char synack[256];
  if(read(rd_fd, synack, sizeof(synack)) == -1) {
    printf("%s\n", strerror(errno));
    return -1;
  }
  printf("Recieved synack from server: %s\n", synack);

  char ack[256];
  snprintf(ack, 255, "%s1", synack);
  printf("Writing ACK to server: %s\n", ack);
  if(write(wr_fd, ack, 255) == -1) {
    printf("Writing ack %s\n", strerror(errno));
    return -1;
  }
  *to_server = wr_fd;
  return rd_fd;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
void server_handshake_half(int *to_client, int from_client) {
  char cl_pid[256];
  int rd_fd = from_client;
  int wr_fd;
  printf("Reading from WKP\n");
  if(read(rd_fd, cl_pid, sizeof(cl_pid)) < 0) {
    printf("Reading %s\n", strerror(errno));
  }

  printf("Opening PP %s\n", cl_pid);
  wr_fd = open(cl_pid, O_WRONLY);
  if(wr_fd == -1) {
    printf("Init write%s\n", strerror(errno));
  }

  char synack[256];
  snprintf(synack, sizeof(synack), "%d", rand());
  printf("Writing synack %s to client\n", synack);
  if(write(wr_fd, synack, strlen(synack)) == -1) {
    printf("Writing %s\n", strerror(errno));
  }

  char ack[256];
  int bytes = read(rd_fd, ack, sizeof(ack));
  if (bytes < 0) {
    printf("%s\n", strerror(errno));
  }
  ack[bytes] = '\0';
  printf("Server received ack %s from client\n", ack);

  char expected[256];
  strcpy(expected, synack);
  strcat(expected, "1");
  if (strcmp(ack, expected) == 0) {
    printf("Handshake Complete, ack was: %s\n", synack);
  } else {
    printf("Error in handshake from client to server\n");
  }

  *to_client = wr_fd;
}
