#pragma once

#include <iostream>
#include <string>

class Channel
{
	public:
		Channel(std::string name);
		Channel(const Channel & src);

		~Channel();

		Channel & operator=(const Channel & src);


	private:
		std::string _name;
		// fd_set userChannels; 
};