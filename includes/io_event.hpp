#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/select.h>
#include <string>
#include <sys/wait.h>
#include <stdio.h>

class io_event {
  public:

    virtual ~io_event(void) {};

    virtual void send_message(void) = 0;
    virtual void receive_message(void) = 0;
    virtual int socket_func(void) const = 0;
    virtual std::string getType(void) = 0;

    private:

};
