#include <Dispatch.hpp>

// Constructors //
Dispatch::Dispatch(void)
: _sigint_received(false), _epollfd(epoll_create1(0)) {
  if (_epollfd == -1) {
    throw ServerException("Error: failed to create epoll instance.");
  }
  // setting signal handler
  setAsSignalHandler();
}

// Destructors //
Dispatch::~Dispatch(void) {};

// Member Functions //
void Dispatch::add(IO_Event& event) {
  struct epoll_event ev;

  // ev.events = EPOLLIN | EPOLLOUT;
  ev.events = EPOLLIN | EPOLLET;
  ev.data.ptr = &event;

  if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, event.getSocket(), &ev) == -1) {
    throw ServerException("Error: failed to add event to epoll instance.");
  }

}

void Dispatch::remove(const IO_Event& event) {
  if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, event.getSocket(), NULL) == -1) {
    perror("epoll_ctl");
    throw ServerException("Error: failed to remove event from epoll instance.");
  } 
}

void Dispatch::run(void) {
  int nfds = epoll_wait(_epollfd, _events, MAX_EVENTS, -1);
  if (nfds == -1) {
    _sigint_received = true;
    // throw ServerException("Error: failed to wait for events.");
  }
  for (int i = 0; i < nfds; ++i) {
    IO_Event *event = static_cast<IO_Event *>(_events[i].data.ptr);
    if (_events[i].events & EPOLLIN) {
      event->receive_message();
    }
    if (_events[i].events & EPOLLOUT) {
      event->send_message();
    }
  }
}

// Getters //

int Dispatch::get_epollfd(void) {
  return _epollfd;
}

int Dispatch::get_sigint_received(void) {
  return _sigint_received;
}

// Signal handler //

void Dispatch::recv_signal(int signal) {
  if (signal == SIGINT) {}
}

void Dispatch::setAsSignalHandler() {
  if (signal(SIGINT, recv_signal) == SIG_ERR)
  {
    perror("signal");
    return ;
  }            
}