
#include "ClientHistory.hpp"


/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/

ClientHistory::ClientHistory(std::string hostname, std::string username,
            std::string realname, std::string nickname) : _hostname(hostname), _username(username),
            _realname(realname), _nickname(nickname) 
{

}



ClientHistory::~ClientHistory()
{

}


/****************************************/
/*                GETTERS               */
/****************************************/

std::string ClientHistory::getHostname(void) const
{
    return (_hostname);
}

std::string ClientHistory::getNickname(void) const {
    return (_nickname);
};

std::string ClientHistory::getUsername(void) const
{
    return (_username);
}

std::string ClientHistory::getRealname(void) const
{
    return (_realname);
}
