#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/select.h>
#include <string>
#include <sys/wait.h>
#include <stdio.h>

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
    ServerManagerException(std::string const &message_val);
    virtual ~ServerManagerException() throw();
    virtual const char *what() const throw();
  private:
    std::string _message;
  };

  int getListenFd();
  void runServer(int listenfd);
  void setAsSignalHandler();
  static void recv_signal(int signal);
  
private:
  ServerManager(void);

  // Private Methods
  void _setupServSock();
  int _closefds(std::vector<int> &fds);

  // Private Attributes
  int _listenfd;
  int _portno;
  std::vector<int> _clientfds;
};
