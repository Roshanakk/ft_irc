#pragma once

#include <string>

#include "AUser.hpp"


class UserRegular : public AUser
{
	public:
		UserRegular(std::string nickname);
		UserRegular(const UserRegular & src);

		UserRegular & operator=(const UserRegular & src);

		~UserRegular();

	private:
		
};