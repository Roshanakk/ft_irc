#pragma once

#include "ServerException.hpp"
#include <iostream>

class ArgParse {
  public:
    static void numArgs(int argc);
    static void parsePort(char *port);
    static void parsePass(char *pass);

  private:
    // Making the constructor private so that the class can't be instantiated
    ArgParse();
    ~ArgParse();
};