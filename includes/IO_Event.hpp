#pragma once

#include <string>

class IO_Event {
  public:

    virtual ~IO_Event(void) {};

    virtual void send_message(void) = 0;
    virtual void receive_message(void) = 0;
    virtual int socket_func(void) const = 0;
    virtual std::string getType(void) = 0;

    private:

};
