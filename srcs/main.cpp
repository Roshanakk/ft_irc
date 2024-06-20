#include "ServerManager.hpp"

int main (int argc, char *argv[]) {
  // Das main
  (void)argc;
  (void)argv;



  try {
    ServerManager serverManager(5001);
  } catch (ServerManager::ServerManagerException &e) {
    std::cerr << "ServerManagerException caught" << std::endl;
  }

  return 0;
}