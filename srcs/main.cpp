#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <vector>
#include <exception>
#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include "ServerException.hpp"


#define MAX_EVENTS 8

class dispatch;
class client;
class server;

class io_event {
  public:

    virtual ~io_event(void) {};

    virtual void send_message(void) = 0;
    virtual void receive_message(void) = 0;
    virtual int socket_func(void) const = 0;
    virtual std::string getType(void) = 0;

  private:

};

class dispatch {
  public:
    dispatch(void)
    : _epollfd(epoll_create1(0)) {
      if (_epollfd == -1) {
        throw serverException("Error: failed to create epoll instance.");
      }
    }

    ~dispatch(void) {};

    void add(io_event& event) {
      struct epoll_event ev;

      // ev.events = EPOLLIN | EPOLLOUT;
      ev.events = EPOLLIN | EPOLLET;
      ev.data.ptr = &event;

      if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, event.socket_func(), &ev) == -1) {
        throw serverException("Error: failed to add event to epoll instance.");
      }
    
    }

    void remove(const io_event& event) {
      if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, event.socket_func(), NULL) == -1) {
        perror("epoll_ctl");
        throw serverException("Error: failed to remove event from epoll instance.");
      } 
    }

    void run(void) {
      int nfds = epoll_wait(_epollfd, _events, MAX_EVENTS, -1);
      if (nfds == -1) {
        perror("epoll_wait failed");
        throw serverException("Error: failed to wait for events.");
      }
      std::cout << "nfds: " << nfds << std::endl;

      for (int i = 0; i < nfds; ++i) {
        std::cout << "i: " << i << std::endl;
        std::cout << "_events[i].data.ptr: " << static_cast<io_event *>(_events[i].data.ptr) << std::endl;
        io_event *event = static_cast<io_event *>(_events[i].data.ptr);
        if (_events[i].events & EPOLLIN) {
          std::cout << "calling receive_message" << std::endl;
          std::cout << "event _type: " << event->getType() << std::endl;
          std::cout << "fd for reading: " << event->socket_func() << std::endl;
          event->receive_message();
        }
        if (_events[i].events & EPOLLOUT) {
          std::cout << "calling send_message" << std::endl;
          std::cout << "event _type: " << event->getType() << std::endl;
          std::cout << "fd for writing: " << event->socket_func() << std::endl;
          event->send_message();
        }
      }
    }

    int get_epollfd(void) {
      return _epollfd;
    }

  private:
    int _epollfd;
    struct epoll_event _events[MAX_EVENTS];
    //std::vector<struct epoll_event> _events;
};

class client : public io_event {
  public:

    client(int sock_val, dispatch& d) : _socket(sock_val), _type("client"), _d(d) {

      if (_socket == -1) {
        throw serverException("Error creating client socket");
      }
    }

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

class server : public io_event {
  public:
    server(void);

    server(dispatch& d) : _type("server"), _d(d) {
      _socket = socket(AF_INET, SOCK_STREAM, 0);
      if (_socket == -1) {
        throw serverException("Error creating server socket");
      }

      _addr.sin_family = AF_INET;
      _addr.sin_port = htons(12345);
      _addr.sin_addr.s_addr = INADDR_ANY;

      int opt = 1;
      if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        throw serverException("Error setting socket options");
      }

      if (bind(_socket, (struct sockaddr *)&_addr, sizeof _addr) == -1) {
        perror("bind");
        throw serverException("Error binding server socket");
      }

      if (listen(_socket, 5) == -1) {
        perror("listen");
        throw serverException("Error listening on server socket");
      }

      _client_addr_len = sizeof _addr;
    };

    ~server(void) {
      std::cout << "server destructor" << std::endl;
      for (size_t i = 0; i < _clients.size(); ++i) {
        delete _clients[i];
      }
    };

    void send_message(void) {}

    void receive_message(void) {
      // struct sockaddr_in addr;
      // int addrlen = sizeof addr;
      struct sockaddr_storage remoteaddr;
      socklen_t addrlen = sizeof remoteaddr;

      int sockfd = accept(_socket, (struct sockaddr *)&remoteaddr, &addrlen);

      std::cout << "calling client constructor" << std::endl;
      client *newClient = new client(sockfd, _d);
      _clients.push_back(newClient);
      std::cout << "adding client to dispatch" << std::endl;
      _d.add(*newClient);
    }

    int socket_func(void) const {
      return _socket;
    };

    std::string getType(void) {
      return _type;
    }

  private:

    // shared_socket
    int _socket;
    std::string _type;
    struct sockaddr_in _addr;
    socklen_t _client_addr_len;
    dispatch& _d;
    std::vector<client *> _clients;

};


int main(void) {

  dispatch d;
  std::cout << "_epollfd: " << d.get_epollfd() << std::endl;
  server s(d);

  d.add(s);

  while (1) {
    d.run();
  }

  return 0;
}