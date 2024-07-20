#pragma once

#include <iostream>
#include <string>
#include <map>

#include "Client.hpp"
#include "ServerReplies.hpp"

class Client;

class Channel {
	public:
		// Constructors & Destructor //
		Channel(std::string name);
		Channel(const Channel & src);
		~Channel();

		// Overloaded operators //
		Channel & operator=(const Channel & src);

		/**********************************************/
		//               Public Methods               //
		/**********************************************/
		// Client methods
		void addClient(Client *client);
		void removeClient(Client *client);
		void promoteClient(Client *client);
		bool checkCanAddMoreClients(void);
		bool shouldDelete(void);
		bool checkIfClientInChannel(Client *client);
		bool checkIfClientIsOp(Client *client);
		// Message methods
		void forwardMessage(std::string message, Client *sender);
		void forwardCommand(std::string message, Client *sender);
		// Key methods
		bool checkKey(std::string& key_val);
		bool requiresKey(void);
		// Invite methods
		bool checkInvite(Client *client);
		void addInvite(Client *client);
		void removeInvite(Client *client);

		// Getters //
		std::string getName(void) const;
		size_t getChanSize(void) const;
		std::map<Client *, bool> &getClients(void);
		bool getInviteOnly(void);
		std::string getTopic() const;
		std::string getClientNicknames(void);

		// Setters //
		void setMaxClients(int maxClients);
		void setInviteOnly(bool inviteOnly);
		void setTopic(std::string topic);
		void setKey(std::string newKey);

	private:
		std::string _name;
		std::string _key;
		std::string _topic;
		std::map<Client *, bool> _clients;
		int _maxClients;
		std::set<Client *> _invites;

		// Mode flags
		bool _inviteOnly;
};