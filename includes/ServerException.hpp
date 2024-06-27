#pragma once

#include <exception>
#include <string>

class ServerException : public std::exception {
  public:

    ServerException(void);
    ServerException(const std::string& msg);
    ~ServerException(void) throw();
    const char *what(void) const throw();

  private:
    std::string _msg;
};