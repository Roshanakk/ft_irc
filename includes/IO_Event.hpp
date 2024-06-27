#pragma once

#include <string>

class IO_Event {
  public:

    virtual ~IO_Event(void) {};

    virtual void send_message(void) = 0;
    virtual void receive_message(void) = 0;
    virtual int getSocket(void) const = 0;

    private:

};
