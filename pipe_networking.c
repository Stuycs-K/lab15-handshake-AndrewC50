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
  //Not used: need PPid in server_handshake
  mkfifo(WKP, 0666);
  int wr_fd = open(WKP, O_RDONLY);
  if(wr_fd == -1) {
    printf("%s\n", strerror(errno));
  }
  remove(WKP);
  return wr_fd;
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
  char cl_pid[256];
  //Block until connection
  int rd_fd = server_setup();
  if(rd_fd == -1) {
    printf("Error Check%s\n", strerror(errno));
    exit(1);
  }
  if(read(rd_fd, cl_pid, sizeof(cl_pid)) < 0) {
    printf("Reading %s\n", strerror(errno));
    exit(1);
  }

  //Open cl_pid, which contains the pid to open for the pp. Opens with write permissions
  // int wr_fd = open(wkp, O_WRONLY);
  int wr_fd = open(cl_pid, O_WRONLY);
  char message[256];
  snprintf(message, sizeof(message), "%d", rand());
  if(write(wr_fd, message, strlen(message)) == -1) {
    printf("Writing %s\n", strerror(errno));
    exit(1);
  }
  if(wr_fd == -1) {
    printf("%s\n", strerror(errno));
  }
  char synack[256];
  printf("Server reading from fd %d\n", rd_fd);
  if (read(rd_fd, synack, sizeof(synack)) < 0) {
    printf("%s\n", strerror(errno));
    exit(1);
  }
  char expected[256];
  strcpy(expected, message);
  strcat(expected, "1");
  if (strcmp(synack, expected) == 0) {
    printf("Handshake Complete, message was: %s\n", synack);
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
  // Write to WKP
  printf("Client handshake\n");
  int wr_fd = open(WKP, O_WRONLY);
  if(wr_fd == -1) {
    printf("%s\n", strerror(errno));
    exit(1);
  }
  int bytes = write(wr_fd, pid, 255);
  printf("Client wrote %d bytes\n", bytes);

  int rd_fd = open(pid, O_RDONLY);
  if(rd_fd == -1) {
    printf("%s\n", strerror(errno));
    exit(1);
  }
  char pp_buff[256];
  if(read(rd_fd, pp_buff, sizeof(pp_buff)) < 0) {
    printf("%s\n", strerror(errno));
    exit(1);
  }
  printf("Recieved message on pp from server: %s\n", pp_buff);
  // int wr_fd = open(pid, O_WRONLY);
  if(wr_fd == -1) {
    printf("%s\n", strerror(errno));
    exit(1);
  }
  char synack[256];
  snprintf(synack, 255, "%s1", pp_buff);
  int bytes_wrote = write(wr_fd, synack, 255);
  if(bytes_wrote == -1) {
    printf("%s\n", strerror(errno));
    exit(1);
  }
  remove(pid);
  *to_server = wr_fd;
  return rd_fd;
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
