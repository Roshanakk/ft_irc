#include "ServerManager.hpp"

// Constructors , Copy Constructor, Destructor
ServerManager::ServerManager(int portno_val) : _portno(portno_val){
  //std::cout << "Constructor called for ServerManager" << std::endl;
  _setupServSock();
  setAsSignalHandler();
  if( -1 == _listenfd )
      throw ServerManagerException();
};

ServerManager::ServerManager(ServerManager const &source) { *this = source; };

ServerManager::~ServerManager(void){
  //std::cout << "Destructor called for ServerManager" << std::endl;
  _closefds(_clientfds);
  
};

// Overloaded Operators
ServerManager &ServerManager::operator=(ServerManager const &rhs) {
  if (this == &rhs)
    return (*this);
  // Additional code here if you need a deep copy.
  return (*this);
};

int ServerManager::getListenFd()
{
  return (_listenfd);
}

void ServerManager::_setupServSock() {
  // Setup the listening socket
  int sockfd;
  struct sockaddr_in serv_addr;
  int opt = 1;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    throw ServerManagerException("Error: failed to open socket.");
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(_portno);
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    throw ServerManagerException("Error: failed to bind.");
  }
  listen(sockfd, 1024);
  _clientfds.push_back(sockfd);
  _listenfd = sockfd;
}

int ServerManager::_closefds(std::vector<int> &fds) {
  for (std::vector<int>::iterator it = fds.begin(); it != fds.end(); ++it) {
    close(*it);
  }
  return 0;
}

// Run server //

void ServerManager::runServer() {
  fd_set master_fds, read_fds, write_fds;
  FD_ZERO(&master_fds);
  FD_ZERO(&read_fds);
  FD_ZERO(&write_fds);
  FD_SET(_listenfd, &master_fds);
  _fdmax = _listenfd;

  while (1) {

    read_fds = master_fds;
    if (select(_fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
      throw ServerManagerException("Error: failed to select.");
      //exit(4);
    }

    // run through the existing connections looking for data to read
    for(int i = 0; i <= _fdmax; i++) {
      if (FD_ISSET(i, &read_fds)) {
        if (i == _listenfd) {
          _acceptNewClient(master_fds);
        } else {
          _readFromClient(i, master_fds);
          FD_SET(i, &write_fds);
        }
      }
      if (FD_ISSET(i, &write_fds) && FD_ISSET(i, &master_fds)) {
        _sendToClient(i, "Your message was received.\n");
        FD_CLR(i, &write_fds);
      }
    }
  }
}

void ServerManager::_acceptNewClient(fd_set &master_fds) {
  struct sockaddr_storage remoteaddr; // client address
  socklen_t addrlen = sizeof remoteaddr;
  int newfd = accept(_listenfd, (struct sockaddr *)&remoteaddr, &addrlen);
  if (newfd == -1) {
    throw ServerManagerException("Error: failed to accept.");
  } else {
    FD_SET(newfd, &master_fds); // add to master set
    if (newfd > _fdmax) {    // keep track of the max
      _fdmax = newfd;
    }
    _clientfds.push_back(newfd);
    std::cout << "selectserver: new connection from " << newfd << std::endl;
  }
}

void ServerManager::_readFromClient(int clientfd, fd_set &master_fds) {
  // read from client
  char buf[256];
  int nbytes = recv(clientfd, buf, sizeof buf, 0);
  if (nbytes <= 0) {
    // got error or connection closed by client
    if (nbytes == 0) {
      // connection closed
      std::cout << "selectserver: socket " << clientfd << " hung up" << std::endl;
    } else {
      throw ServerManagerException("Error: failed to receive.");
    }
    close(clientfd); // close fd of closed connection
    FD_CLR(clientfd, &master_fds); // remove from master set
  } else {
    // we got some data from a client
    buf[nbytes] = '\0';
    std::cout << "Received message (" << clientfd << "): " << buf << std::endl;

  }
}

void ServerManager::_sendToClient(int clientfd, std::string message) {
  const char *response = message.c_str();
  if (send(clientfd, response, strlen(response), 0) == -1) {
    throw ServerManagerException("Error: failed to send.");
  }
}

// ServerManagerException //

ServerManager::ServerManagerException::ServerManagerException() 
  : _message("Error: error not defined.") {};
ServerManager::ServerManagerException::ServerManagerException(std::string const &message_val) 
  : _message(message_val) {};
ServerManager::ServerManagerException::~ServerManagerException() throw(){};
const char *ServerManager::ServerManagerException::what() const throw() {
  return (_message.c_str());
}

// Signal handler //

void ServerManager::recv_signal(int signal) {
  if (signal == SIGINT) {}
}

void ServerManager::setAsSignalHandler() {
  if (signal(SIGINT, recv_signal) == SIG_ERR)
  {
    perror("signal");
    return ;
  }            
}