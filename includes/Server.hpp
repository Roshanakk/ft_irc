#pragma once

#include <netinet/in.h>
#include <cstdio>
#include <sstream>
#include <set>

#include "ServerReplies.hpp"
#include "ServerException.hpp"
#include "AIO_Event.hpp"
#include "Client.hpp"
#include "Dispatch.hpp"
#include "Client.hpp"
#include "ChannelManager.hpp"

class Server : public AIO_Event
{
	public:
		// CONSTRUCTORS & DESTRUCTOR
		Server(Dispatch& d, int port, 
			std::set<Client *>& clients, ChannelManager& cm);
		~Server(void);

		// METHODS
		void send_message(std::string message);
		void receive_message(void);
		int getSocket(void) const;

	private:
		Server(void);

		int _socket;
		int _port;
		struct sockaddr_in _addr;
		socklen_t _client_addr_len;
		Dispatch& _d;
		std::set<Client *>& _clients;
		ChannelManager& _cm;
};
