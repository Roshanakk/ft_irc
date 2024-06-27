#pragma once

#include <sys/socket.h>
#include <unistd.h>

#include "ServerException.hpp"
#include "io_event.hpp"
#include "Dispatch.hpp"

class dispatch;

class client : public io_event 
{
	public:
		//CONSTRUCTORS & DESTRUCTOR
		client(int sock_val, dispatch& d);
		~client(void);

		//METHODS
		void send_message(void);
		void receive_message(void);
		int socket_func(void) const;

		//GETTERS
		std::string getType(void);


	private:
		client(void);

		// shared_socket
		int _socket;
		char buf[1024];
		std::string _type;
		dispatch& _d;

};
