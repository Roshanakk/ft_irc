#pragma once

#include <iostream>
#include <string>
#include <map>
#include <algorithm>

#include "Client.hpp"

#include "CommandException.hpp"

#include "Utilities.hpp"

class Client;

typedef enum e_cmd
{
	CAP, //     0 
	INFO, //    1
	INVITE, //  2 
	JOIN, //    3
	LIST, //    4
	KICK, //    5
	KILL, //    6
	MODE, //    7
	NAMES, //   8
	NICK, //    9
	NOTICE, //  10
	PART, //    11
	PASS, //    12
	PING, //    13
	PRIVMSG, // 14
	QUIT, //    15
	TOPIC, //   16
	USER, //    17
	VERSION, // 18
	WHO, //     19
	WHOIS, //   20
	WHOWAS, //  21
	NB_CMDS //  22
} t_cmd; 

class Command
{
		// typedef void (Command::*_fctCmds)();

	public:
		//CONSTRUCTORS & DESTRUCTOR
		Command(Client & client);
		~Command();

		//GENERAL MANAGING
		// int whatCmd(std::string & line);
		void doCmd(std::string & line);

		//COMMANDS METHODS
		void handle_CAP();
		void handle_INFO();
		void handle_INVITE();
		void handle_JOIN();
		void handle_LIST();
		void handle_KICK();
		void handle_KILL();
		void handle_MODE();
		void handle_NAMES();
		void handle_NICK();
		void handle_NOTICE();
		void handle_PART();
		void handle_PASS();
		void handle_PING();
		void handle_PRIVMSG();
		void handle_QUIT();
		void handle_TOPIC();
		void handle_USER();
		void handle_VERSION();
		void handle_WHO();
		void handle_WHOIS();
		void handle_WHOWAS();


	private:
		std::string _listCmds[22];
		void  (Command::*_fctCmds[22])();
		Client & _client;
		std::string _cmd;
		std::string _parameters;
		//std::vector<std::string> _cmdLine; // First string is the command, second one is the rest,
											// meaning by that : the parameters
};