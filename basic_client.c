#include "pipe_networking.h"

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  int rec = 0;
  while(1) {
    sleep(1);
    srand(time(NULL));
    char message[256];
    snprintf(message, 255, "Hi, my pid is %d, sending message to server", getpid());
    printf("Sending message to server: %s\n", message);
    if(write(to_server, message, strlen(message) + 1) < 0) {
      printf("Error Sending message\n");
      break;
    }
  }
  close(to_server);
  close(from_server);


}
