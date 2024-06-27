#pragma once

#include <sys/epoll.h>
#include <cstdio>
#include <iostream>

#include "ServerException.hpp"
#include "IO_Event.hpp"

#define MAX_EVENTS 8

// class io_event;

class Dispatch {
  public:

    Dispatch(void);
    ~Dispatch(void);

    // Member Functions //
    void add(IO_Event& event);
    void remove(const IO_Event& event);
    void run(void);
    int get_epollfd(void);

  private:
    int _epollfd;
    struct epoll_event _events[MAX_EVENTS];
};