#include "ServerManager.hpp"

int main (int argc, char *argv[]) {
  // Das main
  (void)argc;
  (void)argv;

  try {
    ServerManager serverManager(5001);
    serverManager.runServer();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}