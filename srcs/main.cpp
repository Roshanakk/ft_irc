// #include <sys/socket.h>
// #include <sys/epoll.h>
// #include <arpa/inet.h>
// #include <vector>
// #include <exception>
// #include <string>
#include <iostream>
// #include <unistd.h>
// #include <cstdio>

// #include "io_event.hpp"
// #include "ServerException.hpp"
#include "Dispatch.hpp"
#include "Server.hpp"

int main(void) {

  Dispatch d;
  Server s(d);
  // std::cout << "_epollfd: " << d.get_epollfd() << std::endl;

  d.add(s);

  while (1) {
    d.run();
  }

  return 0;
}