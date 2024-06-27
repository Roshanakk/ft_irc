#pragma once

#include <sys/epoll.h>
#include <cstdio>
#include <iostream>

#include "ServerException.hpp"
#include "io_event.hpp"

#define MAX_EVENTS 8

// class io_event;

class dispatch {
  public:

    dispatch(void);
    ~dispatch(void);

    // Member Functions //
    void add(io_event& event);
    void remove(const io_event& event);
    void run(void);
    int get_epollfd(void);

  private:
    int _epollfd;
    struct epoll_event _events[MAX_EVENTS];
};