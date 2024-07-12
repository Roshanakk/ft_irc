#include "Client.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/


Client::Client(int sock_val, Dispatch& d,
    std::set<Client *> & clients, ChannelManager& cm)
    : _socket(sock_val), _d(d), _clients(clients), _cm(cm), _nickname(""),
        _shouldDelete(false), _passAuth(false), _nickAuth(false), _userAuth(false), 
        _status(PASS_NEEDED) {
    if (_socket == -1) {
        throw ServerException("Error creating client socket");
    }
    _messageStr.clear();
}

Client::~Client(void) {
    std::cout << "Client destructor called" << std::endl;
    _cm.removeClientFromAllChannels(this);
    // If shouldDelete is set to true, then the client has disconnected and we should handle deletion
    // If shouldDelete is false, then the client is being deleted as part of the server shutdown.
    // This way we can avoid double deletion.
    if (_shouldDelete)
        _clients.erase(this);
    close(_socket);
};


/****************************************/
/*                METHODS               */
/****************************************/

std::string Client::getPrefix()
{
    std::string prefix = ":" + _nickname + "!" + _username;
    if (!_hostname.empty())
        prefix = prefix + "@" + _hostname;
    return (prefix);
}   


void Client::send_message(std::string message)
{
    std::cout << "Sending to (" << _socket << "): " << message;
    send(_socket, message.c_str(), message.size(), 0);
}

void Client::receive_message(void)
{
    //Receiving client's message
    memset(buf, 0, sizeof buf);
    int numbytes = recv(_socket, buf, (sizeof buf) - 1, 0);
    if (numbytes < 0)
        throw ServerException("Error: failed to receive.");
    else if (numbytes == 0) //If client disconnected
    {
        std::cout << "Client (" << _socket << ") disconnected" << std::endl;
        _shouldDelete = true;
        _d.remove(*this);
        close(_socket);
        return ;
    }

    _messageStr += buf;
    if (std::find(_messageStr.begin(), _messageStr.end(), '\n') == _messageStr.end())
        return ;



    std::vector<std::string> strVec = Utilities::clientSplit(_messageStr, '\n');
    _messageStr.clear();
    for (std::vector<std::string>::iterator it = strVec.begin(); it != strVec.end(); it++) {
        if (std::find(it->begin(), it->end(), '\n') != it->end())
        {
            // should call parsing function here rather than just printing.
            Command command(*this);
            std::cout << "Received(" << _socket << "): " << *it;
            command.doCmd(*it);
        }
        else {
            // std::cout << "\tAdded remainder: " << *it << std::endl;
            _messageStr = *it;
        }
    }
    // send_message("Message received\n");
}

bool Client::shouldDelete(void) const {
    return (_shouldDelete);
}


/****************************************/
/*                GETTERS               */
/****************************************/

int Client::getSocket(void) const
{
    return (_socket);
}

std::string Client::getHostname(void) const
{
    return (_hostname);
}

std::set<Client *> &Client::getClients(void) const
{
    return (_clients);
}

ChannelManager &Client::getCM(void) const {
    return (_cm);
};

std::string Client::getNickname(void) const {
    return (_nickname);
};

std::string Client::getUsername(void) const
{
    return (_username);
}

std::string Client::getRealname(void) const
{
    return (_realname);
}

bool Client::isAuth(void) const
{
    if (_passAuth && _nickAuth && _userAuth)
        return (true);
    return (false);
}


e_status Client::getStatus(void) const
{
    return (_status);
}

Dispatch& Client::getDispatch() const
{
    return (_d);
}


/*************************************/
/*                SETTERS            */
/*************************************/

void Client::setNickname(std::string nick) {
    _nickname = nick;
};

void Client::setHostname(std::string hostname)
{
    _hostname = hostname;
}

void Client::setUsername(std::string username)
{
    _username = username;
}

void Client::setRealname(std::string realname)
{
    _realname = realname;
}

void Client::setPassAuth(void)
{
    _passAuth = true;
}

void Client::setNickAuth(void)
{
    _nickAuth = true;
}

void Client::setUserAuth(void)
{
    _userAuth = true;
}

void Client::setStatus(e_status status)
{
    _status = status;
}

void Client::setShouldDelete(bool shouldDelete)
{
    _shouldDelete = shouldDelete;
}