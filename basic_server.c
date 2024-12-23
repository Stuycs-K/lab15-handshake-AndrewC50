#include "pipe_networking.h"

int main() {
  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );

  int rec_s = 0;
  int wr_s = 92;
  printf("Fds: to_client: %d from_client %d\n", to_client, from_client);
  read(from_client, &rec_s, 10);
  printf("Recieved message %d from client\n", rec_s);
  // sleep(1);
  write(to_client, &wr_s, 10);
  printf("Wrote message %d to client\n", wr_s);
}
