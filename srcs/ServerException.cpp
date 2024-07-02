#include <ServerException.hpp>
#include "ServerReplies.hpp"

ServerException::ServerException(void) {};

ServerException::ServerException(const std::string& msg) : _msg(msg) {};

ServerException::~ServerException(void) throw() {};

const char *ServerException::what(void) const throw() {
  return _msg.c_str();
}
