#include "pipe_networking.h"

void client_communication(int to_client, int from_client) {
  // while(1) {
  //   srand(time(NULL));
  //   int num = rand() % 101;
  //   printf("Sending %d to client\n", num);
  //   if(write(to_client, &num, sizeof(int)) < 0) {
  //     printf("Client Exited\n");
  //     break;
  //   }
  //   sleep(1);
  // }
  // close(to_client);
  // close(from_client);
  char rec [256];
  while(1) {
    // sleep(1);
    if(read(from_client, rec, sizeof(rec) - 1) <= 0) {
      printf("Connection closed\n");
      break;
    }
    printf("Recieved %s from from client\n", rec);
  }
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
