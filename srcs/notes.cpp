

class io_event {
  public:

    virtual ~io_event(void) {};

    virtual void send(void) = 0;
    virtual void receive(void) = 0;
    virtual int socket(void) const = 0;

  private:

};


class server : public io_event {
  public:

    server(void);
    ~server(void);

    void send(void) {}
    void receive(void) {
      int socket = accept(_socket, (struct sockaddr *)&_client_addr, &_client_addr_len);

      client c(socket);
      dispatch::add(c);

    }
    int socket(void) const;

  private:

    // shared_socket
    int _socket;

};


class client : public io_event {
  public:

    client(void);
    ~client(void);

    void send(void) {
      send(_socket, buf, sizeof buf, 0);
    }
    void receive(void) {
      recv(_socket, buf, sizeof buf, 0);
    }
    int socket(void) const;

  private:

    // shared_socket
    int _socket;

};




class dispatch {
  public:

    dispatch(void)
    : _epollfd(epoll_create1(0)) {
      if (_epollfd == -1) {
        throw std::runtime_error("Error: failed to create epoll instance.");
      }
    }

    ~dispatch(void);

    void add(io_event& event) {

      struct epoll_event ev;

      ev.events = EPOLLIN | EPOLLOUT;
      ev.data.ptr = &event;

      if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, event.socket(), &ev) == -1) {
        throw std::runtime_error("Error: failed to add event to epoll instance.");
      }
    
    }

    void remove(const io_event& event) {

      if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, event.socket(), NULL) == -1) {
        throw std::runtime_error("Error: failed to remove event from epoll instance.");
      } 
    }

    void run(void) {

      int nfds = epoll_wait(_epollfd, _events.data(), _events.size(), -1);

      if (nfds == -1) {
        throw std::runtime_error("Error: failed to wait for events.");
      }

      for (int i = 0; i < nfds; ++i) {
        io_event *event = static_cast<io_event *>(_events[i].data.ptr);
        if (_events[i].events & EPOLLIN) {
          event->receive();
        }
        if (_events[i].events & EPOLLOUT) {
          event->send();
        }
      }

    }

  private:

    int _epollfd;

    std::vector<struct epoll_event> _events;

};




int main(void) {

  server s;
  //dispatch d;

  dispatch::add(s);

  while (is_running) {
    d.run();
  }

  return 0;
}