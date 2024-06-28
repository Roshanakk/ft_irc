#pragma once

#include <netinet/in.h>
#include <vector>
#include <cstdio>
#include <sstream>

#include "ServerException.hpp"
#include "AIO_Event.hpp"
#include "Client.hpp"
#include "Dispatch.hpp"

class Server : public AIO_Event
{
	public:
		// CONSTRUCTORS & DESTRUCTOR
		Server(Dispatch& d, int port);
		~Server(void);

		// METHODS
		void send_message(void);
		void receive_message(void);
		int getSocket(void) const;

	private:
		Server(void);

		int _socket;
		int _port;
		struct sockaddr_in _addr;
		socklen_t _client_addr_len;
		Dispatch& _d;
		std::vector<Client *> _clients;
};
