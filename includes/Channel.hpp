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
		void promoteClient(Client *client);
		bool shouldDelete(void);
		bool checkKey(std::string& key_val);
		bool requiresKey(void);
		bool checkCanAddMoreClients(void);

		// Getters //
		std::string getName(void) const;
		size_t getChanSize(void) const;

		// Setters //
		void setMaxClients(int maxClients);

		// To be implemented
		// void removeClientFromAllChannels(Client *client);
			// remove the client from the channel map
		// int getClinentChannelCount(Client *client);
			// return the number of channels the client is in

	private:
		std::string _name;
		std::string _key;
		std::map<Client *, bool> _clients;
		int _maxClients;
		// Need a ban list
};