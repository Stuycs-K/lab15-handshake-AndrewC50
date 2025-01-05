#include "pipe_networking.h"

void init_server() {
  while(1) {
    int to_client;
    int from_client;

    from_client = server_handshake( &to_client );
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
  init_server();
}
