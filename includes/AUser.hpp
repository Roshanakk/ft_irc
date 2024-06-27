#pragma once

#include <string>

class AUser
{
	public:
		AUser();
		AUser(std::string nickname);
		AUser(const AUser & src);
		virtual ~AUser();

		AUser & operator=(const AUser & src);

		virtual std::string getNickname() const = 0;
		
	protected:
		std::string _nickname;
		int _isOperator;
		// fd_set userChannels; 
};