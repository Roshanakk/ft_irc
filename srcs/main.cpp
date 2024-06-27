#include <iostream>

#include "Dispatch.hpp"
#include "Server.hpp"

int main(void) {

  Dispatch d;
  Server s(d);
  // std::cout << "_epollfd: " << d.get_epollfd() << std::endl;

  d.add(s);

  while (1) {
    d.run();
  }

  return 0;
}