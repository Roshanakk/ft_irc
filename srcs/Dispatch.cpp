#include <Dispatch.hpp>

// Constructors //
Dispatch::Dispatch(void)
: _epollfd(epoll_create1(0)) {
  if (_epollfd == -1) {
    throw ServerException("Error: failed to create epoll instance.");
  }
}

// Destructors //
Dispatch::~Dispatch(void) {};

// Member Functions //
void Dispatch::add(IO_Event& event) {
  struct epoll_event ev;

  // ev.events = EPOLLIN | EPOLLOUT;
  ev.events = EPOLLIN | EPOLLET;
  ev.data.ptr = &event;

  if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, event.socket_func(), &ev) == -1) {
    throw ServerException("Error: failed to add event to epoll instance.");
  }

}

void Dispatch::remove(const IO_Event& event) {
  if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, event.socket_func(), NULL) == -1) {
    perror("epoll_ctl");
    throw ServerException("Error: failed to remove event from epoll instance.");
  } 
}

void Dispatch::run(void) {
  int nfds = epoll_wait(_epollfd, _events, MAX_EVENTS, -1);
  if (nfds == -1) {
    perror("epoll_wait failed");
    throw ServerException("Error: failed to wait for events.");
  }
  // std::cout << "nfds: " << nfds << std::endl;

  for (int i = 0; i < nfds; ++i) {
    // std::cout << "i: " << i << std::endl;
    // std::cout << "_events[i].data.ptr: " << static_cast<IO_Event *>(_events[i].data.ptr) << std::endl;
    IO_Event *event = static_cast<IO_Event *>(_events[i].data.ptr);
    if (_events[i].events & EPOLLIN) {
      // std::cout << "calling receive_message" << std::endl;
      // std::cout << "event _type: " << event->getType() << std::endl;
      // std::cout << "fd for reading: " << event->socket_func() << std::endl;
      event->receive_message();
    }
    if (_events[i].events & EPOLLOUT) {
      // std::cout << "calling send_message" << std::endl;
      // std::cout << "event _type: " << event->getType() << std::endl;
      // std::cout << "fd for writing: " << event->socket_func() << std::endl;
      event->send_message();
    }
  }
}

int Dispatch::get_epollfd(void) {
  return _epollfd;
}