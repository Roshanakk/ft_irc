#pragma once

#include <iostream>
#include <string>
#include <map>

#include "Client.hpp"

class Client;

class Channel {
	public:
		// Constructors & Destructor //
		Channel(std::string name, std::string key_val = "");
		Channel(const Channel & src);
		~Channel();

		// Overloaded operators //
		Channel & operator=(const Channel & src);

		// Public Methods //
		void forwardMessage(std::string message, Client *sender);
		void addClient(Client *client);
		void removeClient(Client *client);
		bool checkKey(std::string& key_val);
		bool shouldDelete(void);
		void promoteClient(Client *client);

	private:
		std::string _name;
		std::string _key;
		std::map<Client *, bool> _clients;
};