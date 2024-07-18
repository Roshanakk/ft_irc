#pragma once

#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <set>

#include "ServerException.hpp"
#include "AIO_Event.hpp"
#include "Dispatch.hpp"
#include "ServerReplies.hpp"
#include "Utilities.hpp"
#include "ChannelManager.hpp"
#include "Command.hpp"
#include "ClientHistory.hpp"

class Dispatch;
class ChannelManager;

typedef std::map<std::string, std::vector<ClientHistory *> > historyMap;

typedef enum e_status
{
	PASS_NEEDED, // no password given yet
	PASS_CORRECT, // pass ok, no user/nick
	PASS_REGISTERED // pass/user/nick ok
} t_status;

class Client : public AIO_Event 
{
	public:
		// CONSTRUCTORS & DESTRUCTOR
		Client(int sock_val, Dispatch& d,
			std::set<Client *>& clients, ChannelManager& cm, historyMap & history);
		~Client(void);

		// METHODS
		std::string getPrefix();
		void send_message(std::string message);
		void receive_message(void);
		bool shouldDelete(void) const;

		//GETTERS
		int getSocket(void) const;
		std::set<Client *> &getClients(void) const;
		ChannelManager &getCM(void) const;
		std::string getHostname(void) const;
		std::string getUsername(void) const;
		std::string getRealname(void) const;
		std::string getNickname(void) const;
		e_status getStatus(void) const;

		historyMap getHistoryMap(void) const;

		bool isAuth(void) const;

		Dispatch& getDispatch() const;



		//SETTERS
		void setHostname(std::string hostname);
		void setUsername(std::string username);
		void setRealname(std::string realname);
		void setNickname(std::string nickname);
		void setStatus(e_status status);
		void setPassAuth(void);
		void setNickAuth(void);
		void setUserAuth(void);
		void setShouldDelete(bool shouldDelete);

		//OPERATOR OVERLOAD TO BE ABLE TO USE MAP
		bool operator <(const Client& rhs) const;
	
		bool operator() (const Client& lhs, const Client& rhs) const
		{
			return lhs._nickname < rhs._nickname;
		}

	private:
		Client(void);
		int _socket;
		char buf[512];
		std::string _messageStr;
		Dispatch& _d;
		std::set<Client *>& _clients;
		ChannelManager& _cm;
		std::string _hostname;
		std::string _username;
		std::string _realname;
		std::string _nickname;
		bool _shouldDelete;

		bool _passAuth;
		bool _nickAuth;
		bool _userAuth;

		e_status _status;

		historyMap & _history;
};
