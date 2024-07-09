#pragma once

#include <exception>
#include <string>
    
    
class CommandException : public std::exception
{
    public:
    CommandException(void);
    CommandException(const std::string & msg);
    ~CommandException() throw();
    virtual const char* what() const throw();

    private:
    std::string _msg;
};