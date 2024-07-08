#include "Channel.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/

Channel::Channel(std::string name, std::string key_val) 
    : _name(name), _key(key_val), _maxClients(-1), _inviteOnly(false) {}

Channel::Channel(const Channel & src) 
    : _name(src._name), _key(src._key), _maxClients(-1) {}

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

// Client methods

void Channel::addClient(Client *client) {
    if (_clients.size() == true)
        _clients.insert(std::make_pair(client, true));
    else
        _clients.insert(std::make_pair(client, false));
};

void Channel::removeClient(Client *client) {
    _clients.erase(client);
};

void Channel::promoteClient(Client *client) {
    std::map<Client*, bool>::iterator it = _clients.find(client);
    if (it != _clients.end()) {
        it->second = true;
    }
};

bool Channel::checkCanAddMoreClients(void) {
    // Answers the question: Can we add more clients to the channel?
    if (_maxClients == -1)
        return (true);
    return (static_cast<int>(_clients.size()) < _maxClients);
};

bool Channel::shouldDelete(void) {
    // return 1 if the channel should be deleted (ie. no clients)
    return (_clients.size() == 0);
};

// Message methods

void Channel::forwardMessage(std::string message, Client *sender) {
    for (std::map<Client*, bool>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->first != sender) {
            std::cout << "Sending message to client: " << it->first->getSocket() << std::endl;
            it->first->send_message("PRIVMSG " + _name + " :" + message + "\r\n");
        }
    }
};

// Key methods

bool Channel::checkKey(std::string& key_val) {
    return (_key == key_val);
};

bool Channel::requiresKey(void) {
    return (_key.size() > 0);
};

// Invite methods

bool Channel::checkInvite(Client *client) {
    return (_invites.find(client) != _invites.end());
};

void Channel::addInvite(Client *client) {
    _invites.insert(client);
};

void Channel::removeInvite(Client *client) {
    _invites.erase(client);
};

// Ban methods

bool Channel::checkBan(Client *client) {
    return (_bans.find(client) != _bans.end());
};

void Channel::addBan(Client *client) {
    _bans.insert(client);
};

void Channel::removeBan(Client *client) {
    _bans.erase(client);
};

/**********************************************************/
/*                        GETTERS                         */
/**********************************************************/

std::string Channel::getName(void) const {
    return (_name);
};


size_t Channel::getChanSize(void) const {
    return (_clients.size());
};

std::map<Client *, bool> &Channel::getClients(void) {
    return (_clients);
};

bool Channel::getInviteOnly(void) {
    return (_inviteOnly);
};

/**********************************************************/
/*                        SETTERS                         */
/**********************************************************/

void Channel::setMaxClients(int maxClients) {
    if (maxClients < 0) {
        // maxClients < 0 means no limit
        _maxClients = -1;
        return ;
    } else if (maxClients > 100000) {
        // Sets the max number of clients allowed in the channel to 100000
        _maxClients = 100000;
        return ;
    } else {
        // sets the max number of clients allowed in the channel
        _maxClients = maxClients;
    }
};

void Channel::setInviteOnly(bool inviteOnly) {
    _inviteOnly = inviteOnly;
};