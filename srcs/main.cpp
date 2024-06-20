#include "ServerManager.hpp"

int main (int argc, char *argv[]) {
  // Das main
  (void)argc;
  (void)argv;



  try {
    ServerManager serverManager(5001);
    int listenfd = serverManager.getListenFd();
    serverManager._runServer(listenfd);
  } catch (std::exception &e) {
    std::cerr << "ServerManagerException caught" << std::endl;
  }

  return 0;
}