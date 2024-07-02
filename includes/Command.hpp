#pragma once

#include <iostream>
#include <string>
#include <set>

#include "AUser.hpp"
#include "Client.hpp"

typedef enum e_cmd
{
  CAP, //
  INFO, //
  INVITE, //
  JOIN, //
  LIST, //
  KICK, //
  KILL, //
  MODE, //
  NAMES, //
  NICK, //
  NOTICE, //
  PART, //
  PING, //
  PRIMSG, //
  TOPIC, //
  USER, //
  VERSION, //
  WHO, //
  WHOIS, //
  WHOWAS, //
  NB_CMDS //
} t_cmd; 

class Command {
  public:
    //CONSTRUCTORS & DESTRUCTOR
    Command(Client & client);
    ~Command();

    //COMMANDS METHODS
    // void doNICK(std::string & nickname, AUser * user);
    int whatCmd(std::string & line);
    // void doCmd(std::string & line);



  private:
    std::string _listCmds[21];
    Client & _client;
};