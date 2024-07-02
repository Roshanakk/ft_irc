#pragma once

// #include <iostream>
#include <string>

typedef enum e_cmd
{
  NICK,
  USER
} t_cmd; 

class CmdParse {
  public:
    static void doNICK(std::string nickname);

  private:
    // Making the constructor private so that the class can't be instantiated
    CmdParse();
    ~CmdParse();
};