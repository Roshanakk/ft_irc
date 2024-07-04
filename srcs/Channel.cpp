#include "Channel.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/

Channel::Channel(std::string name, std::string key_val) 
    : _name(name), _key(key_val) {}

Channel::Channel(const Channel & src) 
    : _name(src._name), _key(src._key) {}

Channel::~Channel() {}


/**********************************************************/
/*                   OPERATORS OVERLOAD                   */
/**********************************************************/

Channel & Channel::operator=(const Channel & src)
{
    if (this != &src)
        *this = src;
    return (*this);
}

/**********************************************************/
/*                      PUBLIC METHODS                    */
/**********************************************************/

void Channel::forwardMessage(std::string message, Client *sender) {
    for (std::map<Client*, bool>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->first != sender) {
            it->first->send_message("PRIVMSG #" + _name + " " + message + "\r\n");
        }
    }
};

void Channel::addClient(Client *client) {
    if (_clients.size() == true)
        _clients.insert(std::make_pair(client, true));
    else
        _clients.insert(std::make_pair(client, false));
};

void Channel::removeClient(Client *client) {
    _clients.erase(client);
};

bool Channel::checkKey(std::string& key_val) {
    return (_key == key_val);
};

bool Channel::shouldDelete(void) {
    // return 1 if the channel should be deleted (ie. no clients)
    return (_clients.size() == 0);
};

void Channel::promoteClient(Client *client) {
    std::map<Client*, bool>::iterator it = _clients.find(client);
    if (it != _clients.end()) {
        it->second = true;
    }
};