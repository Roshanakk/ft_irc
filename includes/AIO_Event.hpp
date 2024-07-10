#pragma once

#include <string>

class AIO_Event {
  public:

    virtual ~AIO_Event(void) {};

    virtual void send_message(std::string message) = 0;
    virtual void receive_message(void) = 0;
    virtual int getSocket(void) const = 0;
    virtual bool shouldDelete(void) const = 0;

  private:
};
