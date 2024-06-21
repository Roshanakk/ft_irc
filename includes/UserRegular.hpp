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

#include "AUser.hpp"


class UserRegular : public AUser
{
	public:
		UserRegular();
		UserRegular(const UserRegular & src);

		UserRegular & operator=(const UserRegular & src);

		~UserRegular();

	private:
		
};