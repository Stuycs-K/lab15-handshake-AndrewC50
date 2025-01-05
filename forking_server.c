#include "pipe_networking.h"

void client_communication(int to_client, int from_client) {
  while(1) {
    srand(time(NULL));
    int num = rand() % 101;
    printf("Sending %d to client\n", num);
    if(write(to_client, &num, sizeof(int)) < 0) {
      printf("Client Exited\n");
      break;
    }
    sleep(1);
  }
  close(to_client);
  close(from_client);
}

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
  if (read(rd_fd, ack, sizeof(ack)) < 0) {
    printf("%s\n", strerror(errno));
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
}

static void sighandler (int signo) {
  if (signo == SIGINT) {
    remove(WKP);
    exit(1);
  }

  if (signo == SIGPIPE) {
    // Do nothing
  }
}

int main() {
  signal(SIGINT, sighandler);
  signal(SIGPIPE, sighandler);
  int to_client;
  int from_client;
  while(1) {
    from_client = server_setup();
    pid_t p = fork();
    if (p < 0) {
      printf("Forking error %s\n", strerror(errno));
      exit(1);
    }
    if (p == 0) {
      server_handshake_half(&to_client, from_client);
      client_communication(to_client, from_client);
      exit(1);
    } else {
      close(to_client);
      close(from_client);
    }
  }
}
