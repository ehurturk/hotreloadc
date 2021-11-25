#include "bootstrapper/bootstrapper.h"
#include <unistd.h>

int main(int argc, char **argv) {
  printf("Starting the app...\n");
  init_plugins();
  for (int i = 0; i < 10; i++) {
    update_plugins();
    sleep(1);
  }
  delete_bootstrapper();
}
