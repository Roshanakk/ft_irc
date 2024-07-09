#include "CommandException.hpp"

CommandException::CommandException(void) {}


CommandException::CommandException(const std::string & msg) : _msg(msg) {}


CommandException::~CommandException() throw() {}


const char* CommandException::what() const throw()
{
    return (_msg.c_str());
}