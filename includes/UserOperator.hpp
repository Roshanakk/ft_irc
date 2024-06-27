#pragma once

// #include <iostream>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <cstring>
// #include <vector>
// #include <unistd.h>
// #include <sys/select.h>
#include <string>
// #include <sys/wait.h>
// #include <stdio.h>

#include "AUser.hpp"

class UserOperator : public AUser
{
	public:
		UserOperator(std::string nickname);
		UserOperator(const UserOperator & src);

		UserOperator & operator=(const UserOperator & src);

		~UserOperator();
		////////// COMMANDS :
		// KICK    - Eject a client from the channel
        // MODE    - Change the channel's mode
        // INVITE  - Invite a client to an invite-only channel (mode +i)
        // TOPIC   - Change the channel topic in a mode +t channel

	private:
		
};