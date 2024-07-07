#pragma once

#include <iostream>
#include <string>
#include <map>
#include <algorithm>

#include "Client.hpp"
#include "NoCommandException.hpp"
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
	PING, //    12
	PRIVMSG, //  13
	TOPIC, //   14
	USER, //    15
	VERSION, // 16
	WHO, //     17
	WHOIS, //   18
	WHOWAS, //  19
	NB_CMDS //  20
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
		void handle_PING();
		void handle_PRIVMSG();
		void handle_TOPIC();
		void handle_USER();
		void handle_VERSION();
		void handle_WHO();
		void handle_WHOIS();
		void handle_WHOWAS();


	private:
		std::string _listCmds[21];
		void  (Command::*_fctCmds[21])();
		Client & _client;
		std::vector<std::string> _cmdLine; // First string is the command, second one is the rest,
											// meaning by that : the parameters

};