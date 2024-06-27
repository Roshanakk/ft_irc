#pragma once

#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

#include "ServerException.hpp"
#include "IO_Event.hpp"
#include "Dispatch.hpp"
#include "ServerReplies.hpp"

class Dispatch;

class Client : public IO_Event 
{
	public:
		//CONSTRUCTORS & DESTRUCTOR
		Client(int sock_val, Dispatch& d);
		~Client(void);

		//METHODS
		void send_message(void);
		void receive_message(void);
		int getSocket(void) const;

		//GETTERS
		std::string getType(void);

	private:
		Client(void);

		// shared_socket
		int _socket;
		char buf[1024];
		std::string _type;
		Dispatch& _d;

};
