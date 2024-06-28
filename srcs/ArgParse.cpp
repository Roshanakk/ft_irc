#include "ArgParse.hpp"

ArgParse::~ArgParse() {};

void ArgParse::numArgs(int argc) {
  if (argc != 3) {
    throw ServerException("Usage: ./ircserv [port] [password]");
  }
}

int ArgParse::parsePort(char *portStr) {
  // Parse the argument for the port
  for (char* p = portStr; *p; ++p) {
    if (!std::isdigit(*p)) {
      throw ServerException("Port must be a (pos) number.");
    }
  }

  // Convert string to long
  char* end;
  long port = std::strtol(portStr, &end, 10);

  // Check for valid port range
  if (port < 1 || port > 65535) {
    throw ServerException("Port number must be between 1 and 65535.");
  }

  return static_cast<int>(port);
}

std::string ArgParse::parsePass(char *pass) {
  // Parse the argument for the password
  std::string passStr(pass);
  if (passStr.size() < 1) {
    throw ServerException("Password must be at least 1 character long.");
  }
  return passStr;
}