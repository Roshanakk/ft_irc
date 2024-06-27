#pragma once

#include <sys/epoll.h>
#include <cstdio>
#include <iostream>
#include <signal.h>

#include "ServerException.hpp"
#include "AIO_Event.hpp"

#define MAX_EVENTS 8

// class AIO_Event;

class Dispatch {
  public:

    Dispatch(void);
    ~Dispatch(void);

    // Member Functions //
    void add(AIO_Event& event);
    void remove(const AIO_Event& event);
    void run(void);
    int get_epollfd(void);
    int get_sigint_received(void);
		static void recv_signal(int signal);
		void setAsSignalHandler();

  private:
    bool _sigint_received;
    int _epollfd;
    struct epoll_event _events[MAX_EVENTS];
};