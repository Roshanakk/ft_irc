#include <iostream>

#include "Dispatch.hpp"
#include "Server.hpp"
#include "ArgParse.hpp"

int main(int argc, char **argv) {

  try {
    ArgParse::numArgs(argc);
    ArgParse::parsePort(argv[1]);
    ArgParse::parsePass(argv[2]);
  } catch (const ServerException& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

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