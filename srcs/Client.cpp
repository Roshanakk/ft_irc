#include "Client.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/


Client::Client(int sock_val, Dispatch& d,
    std::set<Client *> & clients, ChannelManager& cm)
    : _socket(sock_val), _d(d), _clients(clients), _cm(cm)
{
    if (_socket == -1) {
        throw ServerException("Error creating client socket");
    }
    _messageStr.clear();
}

Client::~Client(void) {
    std::cout << "Client destructor called" << std::endl;
    close(_socket);
};


/****************************************/
/*                METHODS               */
/****************************************/

void Client::send_message(std::string message)
{
    // std::string response = "Message Received\n";
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
        _d.remove(*this);
        close(_socket);
        return ;
    }

    _messageStr += buf;
    if (std::find(_messageStr.begin(), _messageStr.end(), '\n') == _messageStr.end())
        return ;

    Command command(*this);


    std::vector<std::string> strVec = Utilities::clientSplit(_messageStr, '\n');
    _messageStr.clear();
    for (std::vector<std::string>::iterator it = strVec.begin(); it != strVec.end(); it++) {
        if (std::find(it->begin(), it->end(), '\n') != it->end())
        {
            // should call parsing function here rather than just printing.
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

int Client::getSocket(void) const
{
    return (_socket);
}

std::string Client::getHostname(void) const
{
    return (_hostname);
}

std::set<Client *> &Client::getClinents(void) const
{
    return (_clients);
}

ChannelManager &Client::getCM(void) const {
    return (_cm);
};