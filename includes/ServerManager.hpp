#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/select.h>

class ServerManager {
public:
  // Constructors , Copy Constructor, Destructor
  ServerManager(int portno_val);
  ServerManager(ServerManager const &source);
  ~ServerManager(void);
  
  // Overloaded Operators
  ServerManager &operator=(ServerManager const &rhs);

  class ServerManagerException : public std::exception {
  public:
    ServerManagerException();
    virtual ~ServerManagerException() throw();
    virtual const char *what() const throw();
  };

private:
  ServerManager(void);

  // Private Methods
  int _setupServSock();
  int _closefds(std::vector<int> &fds);
  void _runServer(int listenfd);

  // Private Attributes
  int _portno;
  std::vector<int> _clientfds;
};
