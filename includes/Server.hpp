#pragma once

#include <netinet/in.h>
#include <vector>
#include <cstdio>

#include "ServerException.hpp"
#include "IO_Event.hpp"
#include "Client.hpp"
#include "Dispatch.hpp"

class Server : public IO_Event
{
	public:
		//CONSTRUCTORS & DESTRUCTOR
		Server(void);
		Server(Dispatch& d);
		~Server(void);

		//METHODS
		void send_message(void) {}
		void receive_message(void);
		int socket_func(void) const;

		//GETTERS
		std::string getType(void);


	private:
		// shared_socket
		int _socket;
		std::string _type;
		struct sockaddr_in _addr;
		socklen_t _client_addr_len;
		Dispatch& _d;
		std::vector<Client *> _clients;

};
