#include <iostream>

#include "Dispatch.hpp"
#include "Server.hpp"
#include "ArgParse.hpp"

int main(int argc, char *argv) {

  if (argc != 3) {
    std::cerr << "Usage: ./ircserv [port] [password]" << std::endl;
    return 1;
  }

  try {
    ArgParse::parsePort();
    ArgParse::parsePass();
  } catch (const ArgParseException& e) {
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