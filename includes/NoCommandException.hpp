#pragma once

#include <exception>
#include <string>
    
    
class NoCommandException : public std::exception
{
    public:
    NoCommandException(void);
    NoCommandException(const std::string & msg);
    ~NoCommandException() throw();
    virtual const char* what() const throw();

    private:
    std::string _msg;
};