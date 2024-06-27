#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <vector>
#include <exception>
#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include "ServerException.hpp"


#define MAX_EVENTS 8

class dispatch;
class client;
class server;




int main(void) {

  dispatch d;
  std::cout << "_epollfd: " << d.get_epollfd() << std::endl;
  server s(d);

  d.add(s);

  while (1) {
    d.run();
  }

  return 0;
}