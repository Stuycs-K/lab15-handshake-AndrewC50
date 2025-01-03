#include "pipe_networking.h"
#include <time.h>

static void sighandler (int signo) {
  if (signo == SIGINT) {
    remove(WKP);
  }
  exit(1);
}
int main() {
  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );

  // int rec_s = 0;
  // int wr_s = 92;
  // read(from_client, &rec_s, sizeof(int));
  // printf("Recieved message %d from client\n", rec_s);
  // sleep(1);
  // write(to_client, &wr_s, sizeof(int));
  // printf("Wrote message %d to client\n", wr_s);
  signal(SIGINT, sighandler);
  while(1) {
    srand(time(NULL));
    int num = rand() % 101;
    printf("Sending %d to client\n", num);
    write(to_client, &num, sizeof(int));
    sleep(1);
  }
}
