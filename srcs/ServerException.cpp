#include <ServerException.hpp>

serverException::serverException(void) {};

serverException::serverException(const std::string& msg) : _msg(msg) {};

serverException::~serverException(void) throw() {};

const char *serverException::what(void) const throw() {
  return _msg.c_str();
}