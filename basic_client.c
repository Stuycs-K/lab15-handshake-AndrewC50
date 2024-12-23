#include "pipe_networking.h"

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  int wr = 5;
  int rec = 0;
  printf("Writing %d to fd %d\n", wr, to_server);
  write(to_server, &wr, sizeof(int));
  // sleep(1);
  printf("Reading from fd %d\n", from_server);
  read(from_server, &rec, sizeof(int));
  printf("Read %d\n", rec);
}
