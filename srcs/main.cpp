#include <iostream>

#include "Dispatch.hpp"
#include "Server.hpp"

int main(void) {

  Dispatch d;
  Server s(d);
  // std::cout << "_epollfd: " << d.get_epollfd() << std::endl;

  d.add(s);

  while (!d.get_sigint_received()) {
    try {
      d.run();
    } catch (const ServerException& e) {
      std::cerr << e.what() << std::endl;
    }
  }

  return 0;
}