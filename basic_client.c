#include "pipe_networking.h"

static void sighandler (int signo) {
  if (signo == SIGINT) {
    remove(WKP);
  }
  exit(1);
}

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  //
  // int wr = 5;
  // int rec = 0;
  // printf("Writing %d\n", wr, to_server);
  // write(to_server, &wr, sizeof(int));
  // sleep(1);
  // read(from_server, &rec, sizeof(int));
  // printf("Read %d\n", rec);
  int rec = 0;
  signal(SIGINT, sighandler);
  while(1) {
    // sleep(1);
    if(read(from_server, &rec, sizeof(int)) == -1) {
      break;
    }
    printf("Recieved %d from server\n", rec);
  }
}
