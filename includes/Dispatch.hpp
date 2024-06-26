

class Dispatch {
  public:

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