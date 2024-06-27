#include <iostream>

#include "Dispatch.hpp"
#include "Server.hpp"

int main(void) {

  Dispatch d;
  Server s(d);

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