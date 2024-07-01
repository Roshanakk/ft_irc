#pragma once

#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <algorithm>

#include "ServerException.hpp"
#include "AIO_Event.hpp"
#include "Dispatch.hpp"
#include "ServerReplies.hpp"

class Dispatch;

class Client : public AIO_Event 
{
	public:
		// CONSTRUCTORS & DESTRUCTOR
		Client(int sock_val, Dispatch& d);
		~Client(void);

		// METHODS
		void send_message(void);
		void receive_message(void);
		int getSocket(void) const;

	private:
		Client(void);

		int _socket;
		char buf[1024];
		Dispatch& _d;

};
