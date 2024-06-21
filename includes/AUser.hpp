#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/select.h>
#include <string>
#include <sys/wait.h>
#include <stdio.h>

# define MAX_CHANNELS 10

class AUser
{
	public:
		AUser();
		AUser(const AUser & src);
		AUser(std::string nickname);
		virtual ~AUser();

		AUser & operator=(const AUser & src);

		virtual std::string getNickname() const = 0;
		
	protected:
		std::string _nickname;
		int _isOperator;
		// fd_set userChannels; 

};