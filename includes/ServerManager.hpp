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

  void runServer();
  void setAsSignalHandler();
  static void recv_signal(int signal);
  
private:
  ServerManager(void);

  // Private Methods
  void _setupServSock();
  int _closefds(std::vector<int> &fds);
  void _readFromClient(int clientfd, fd_set &master_fds);
  void _acceptNewClient(fd_set &master_fds);
  void _sendToClient(int clientfd, std::string message);

  // Private Attributes
  int _listenfd;
  int _portno;
  int _fdmax;
  std::vector<int> _clientfds;
};
