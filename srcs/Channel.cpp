#include "Channel.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/

Channel::Channel(std::string name) 
    : _name(name), _key(""), _topic(""), _maxClients(-1), _topicSetter(NULL),
    _inviteOnly(false), _onlyOperTopic(false) {}

Channel::Channel(const Channel & src) 
    : _name(src._name), _key(src._key), _maxClients(-1) {}

Channel::~Channel() {
    std::cout << "Channel destructor called" << std::endl;
}


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
    if (client == NULL)
        return ;

    if (_clients.size() == false)
        // add the client as a channel operator
        _clients.insert(std::make_pair(client, true));
    else
        _clients.insert(std::make_pair(client, false));
    
    this->removeInvite(client);
    if (this->getTopic().size() > 0) {
        client->send_message(RPL_TOPIC(client->getPrefix(), this->getName(), this->getTopic()));
    } else {
        client->send_message(RPL_NOTOPIC(this->getName()));
    }
    client->send_message(RPL_NAMREPLY(_name, this->getClientNicknames()));
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

bool Channel::checkIfClientInChannel(Client *client) {
    return (_clients.find(client) != _clients.end());
};

bool Channel::checkIfClientIsOp(Client *client) {
    std::map<Client*, bool>::iterator it = _clients.find(client);
    if (it != _clients.end()) {
        return (it->second);
    }
    return (false);
};

// Message methods

void Channel::forwardMessage(std::string message, Client *sender) {
    for (std::map<Client*, bool>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->first != sender) {
            std::cout << "Sending message to client: " << it->first->getSocket() << std::endl;
            it->first->send_message(":" \
                + sender->getNickname() + " PRIVMSG " + _name + " :" \
                + message + "\r\n");
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


//Operator method

bool Channel::checkIfClientOperator(Client *client)
{
    return (_clients[client]);
}

//Ban methods

void Channel::banUser(Client *client)
{
    _bannedClients.insert(client);
    removeClient(client);
}

// bool Channel::isBanned(Client *client)
// {
//     if (find(_bannedClients.begin(), _bannedClients.end(), client) != _bannedClients.end())
//         return (true);
//     return (false);
// }


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

std::string Channel::getTopic() const {
    return (_topic);
};

std::string Channel::getClientNicknames(void) {
    std::ostringstream ss;
    for (std::map<Client*, bool>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it != _clients.begin())
            ss << ", ";
        ss << it->first->getNickname();
    }
    return ss.str();
};


bool Channel::onlyOperCanChangeTopic(void) const
{
    return(_onlyOperTopic);
}

Client * Channel::getTopicSetter(void) const
{
    return (_topicSetter);
}


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

void Channel::setTopic(std::string topic) {
    _topic = topic;
};

void Channel::setTopicSetter(Client * topicSetter)
{
    _topicSetter = topicSetter;
}