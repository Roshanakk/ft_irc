#pragma once

#include <string>

class ClientHistory
{
    public:
        ClientHistory(std::string hostname, std::string username,
            std::string realname, std::string nickname);
        ~ClientHistory();

		//GETTERS
		std::string getHostname(void) const;
		std::string getUsername(void) const;
		std::string getRealname(void) const;
		std::string getNickname(void) const;

		// //SETTERS
		// void setHostname(std::string hostname);
		// void setUsername(std::string username);
		// void setRealname(std::string realname);
		// void setNickname(std::string nickname);

    private:
        // ClientsHistory(void);
		std::string _hostname;
		std::string _username;
		std::string _realname;
		std::string _nickname;
};