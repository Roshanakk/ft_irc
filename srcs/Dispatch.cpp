#include <Dispatch.hpp>

// Constructors //
dispatch::dispatch(void)
: _epollfd(epoll_create1(0)) {
  if (_epollfd == -1) {
    throw serverException("Error: failed to create epoll instance.");
  }
}

// Destructors //
~dispatch(void) {};

// Member Functions //
void add(io_event& event) {
  struct epoll_event ev;

  // ev.events = EPOLLIN | EPOLLOUT;
  ev.events = EPOLLIN | EPOLLET;
  ev.data.ptr = &event;

  if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, event.socket_func(), &ev) == -1) {
    throw serverException("Error: failed to add event to epoll instance.");
  }

}