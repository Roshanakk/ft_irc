#pragma once

#include <exception>
#include <string>

class serverException : public std::exception {
  public:

    serverException(void);
    serverException(const std::string& msg);
    ~serverException(void) throw();
    const char *what(void) const throw();

  private:
    std::string _msg;
};