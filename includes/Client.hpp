#pragma once

#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <set>

#include "ServerException.hpp"
#include "AIO_Event.hpp"
#include "Dispatch.hpp"
#include "ServerReplies.hpp"
#include "Utilities.hpp"
#include "Channel.hpp"

class Dispatch;

class Client : public AIO_Event 
{
	public:
		// CONSTRUCTORS & DESTRUCTOR
		Client(int sock_val, Dispatch& d,
			std::set<Client *>& clients, std::set<Channel *>& channels);
		~Client(void);

		// METHODS
		void send_message(std::string message);
		void receive_message(void);

		//GETTERS & SETTERS
		int getSocket(void) const;
		std::string getHostname(void) const;
		std::string getUsername(void) const;
		std::string getRealname(void) const;

		void setHostname(std::string hostname);
		void setUsername(std::string username);
		void setRealname(std::string realname);

	private:
		Client(void);
		int _socket;
		char buf[512];
		std::string _messageStr;
		Dispatch& _d;
		std::set<Client *>& _clients;
		std::set<Channel *>& _channels;

		std::string _hostname;
		std::string _username;
		std::string _realname;

		std::string _nickname;

};
