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

#include "io_event.hpp"

class client : public io_event 
{
    public:

    client(int sock_val, dispatch& d);

    ~client(void) {
      std::cout << "client destructor" << std::endl;
    };

    void send_message(void) {
      std::cout << "client send_message" << std::endl;
      send(_socket, buf, sizeof buf, 0);
    }

    void receive_message(void) {
      std::cout << "client receive_message" << std::endl;
      int numbytes = recv(_socket, buf, sizeof buf, 0);
      if (numbytes < 0)
        throw serverException("Error: failed to receive.");
      else if (numbytes == 0) {
        std::cout << "client disconnected" << std::endl;
        _d.remove(*this);
        close(_socket);
        return ;
      }
      std::cout << "Received: " << buf << std::endl;
    }
    int socket_func(void) const {
      return _socket;
    };

    std::string getType(void) {
      return _type;
    }

  private:
    client(void);

    // shared_socket
    int _socket;
    char buf[1024];
    std::string _type;
    dispatch& _d;

};
