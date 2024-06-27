#include "ServerException.hpp"
#include <sys/epoll.h>
#include <iostream>

#define MAX_EVENTS 8

class dispatch {
  public:

    dispatch(void);
    ~dispatch(void) {};

    // Member Functions //
    void add(io_event& event);
    void remove(const io_event& event);
    void run(void);
    int get_epollfd(void);

  private:
    int _epollfd;
    struct epoll_event _events[MAX_EVENTS];
};