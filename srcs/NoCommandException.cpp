#include "NoCommandException.hpp"

NoCommandException::NoCommandException(void) {}


NoCommandException::NoCommandException(const std::string & msg) : _msg(msg) {}


NoCommandException::~NoCommandException() throw() {}


const char* NoCommandException::what() const throw()
{
    return (_msg.c_str());
}