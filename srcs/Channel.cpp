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

    if (_clients.size() == 0)
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
    client->send_message(RPL_NAMREPLY(client->getNickname(), _name, this->getClientNicknames()));
    client->send_message(RPL_ENDOFNAMES(client->getNickname(), _name));
    this->forwardCommand(":" + client->getPrefix() + " JOIN " + _name + "\r\n", client);
};

void Channel::removeClient(Client *client) {
    _clients.erase(client);
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
    // Check null
    if (client == NULL) {
        return (false);
    }
    std::map<Client*, bool>::iterator it = _clients.find(client);
    // Check if client is in the channel
    if (it == _clients.end()) {
        return (false);
    }
    // Only return true if the client is an operator
    if (it != _clients.end()) {
        return (it->second);
    }
    // catchall
    return (false);
};

// Message methods

void Channel::forwardMessage(std::string message, Client *sender) {
    for (std::map<Client*, bool>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->first != sender) {
            std::cout << "Sending message to client: " << it->first->getSocket() << std::endl;
            it->first->send_message(sender->getPrefix() + " PRIVMSG " + _name + " :" \
                + message + "\r\n");
        }
    }
};

void Channel::forwardCommand(std::string message, Client *sender) {
    for (std::map<Client*, bool>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->first != sender) {
            std::cout << "Sending command to client: " << it->first->getSocket() << std::endl;
            it->first->send_message(message);
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

// void Channel::banUser(Client *client)
// {
//     _bannedClients.insert(client);
//     removeClient(client);
// }

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
            ss << " ";
        if (it->second)
            ss << "@";
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


bool Channel::getOnlyOperTopic(void) const {
    return (_onlyOperTopic);
};

int Channel::getMaxClients(void) const {
    return (_maxClients);
};

std::string Channel::getMode() const {
    std::string mode = "";
    if (_inviteOnly)
        mode += "i";
    if (_onlyOperTopic)
        mode += "t";
    if (_key.size() > 0)
        mode += "k";
    if (_maxClients != -1)
        mode += "l";
    if (mode.size() > 0)
        mode = "+" + mode;
    return mode;
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

void Channel::setTopic(std::string topic) {
    _topic = topic;
};

void Channel::setKey(std::string newKey) {
    _key = newKey;
};
void Channel::setTopicSetter(Client * topicSetter)
{
    _topicSetter = topicSetter;
}

void Channel::setOnlyOperTopic(bool operTopic) {
    _onlyOperTopic = operTopic;
};

void Channel::setOperStatus(Client *client, bool status) {
    // Check null
    if (client == NULL)
        return ;
    // look for the client in the channel
    if (_clients.find(client) != _clients.end())
        _clients[client] = status;
    else {
        // if not found, just return
        return ;
    }
};