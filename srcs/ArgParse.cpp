#include "ArgParse.hpp"

ArgParse::~ArgParse() {};

void ArgParse::numArgs(int argc) {
  if (argc != 3) {
    throw ServerException("Usage: ./ircserv [port] [password]");
  }
}

void ArgParse::parsePort(char *port) {
  // Parse the argument for the port
  (void) port;
}

void ArgParse::parsePass(char *pass) {
  // Parse the argument for the password
  (void) pass;
}