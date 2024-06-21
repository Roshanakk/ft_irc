#pragma once

#include <iostream>
#include <string>


class Channel
{
	public:
		Channel(std::string name);
		Channel(const Channel & src);

		Channel & operator=(const Channel & src);

		~Channel();

	private:
		std::string _name;
		// fd_set userChannels; 

};