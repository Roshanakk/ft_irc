#include <Dispatch.hpp>
#include <sys/epoll.h>

Dispatch::Dispatch(void)
: _epollfd(epoll_create1(0)) {
  if (_epollfd == -1) {
    throw serverException("Error: failed to create epoll instance.");
  }
}