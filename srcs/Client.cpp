#include "Client.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/


Client::Client(int sock_val, Dispatch& d) 
    : _socket(sock_val), _d(d) 
{
    if (_socket == -1) {
    throw ServerException("Error creating client socket");
    }
}

Client::~Client(void) {
    std::cout << "Client destructor called" << std::endl;
    close(_socket);
};


/****************************************/
/*                METHODS               */
/****************************************/

void Client::send_message(void)
{
    std::string response = "Message Received\n";
    send(_socket, response.c_str(), response.size(), 0);
}

void Client::receive_message(void)
{
    //Receiving client's message
    memset(buf, 0, sizeof buf);
    int numbytes = recv(_socket, buf, sizeof buf, 0);
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
    std::vector<std::string> strVec = Utilities::split(_messageStr, '\n');
    for (std::vector<std::string>::iterator it = strVec.begin(); it != strVec.end(); it++) {
        // should call parsing function here rather than just printing.
        std::cout << "Received(" << _socket << "): " << *it << std::endl;
    }
    _messageStr.clear();
    send_message();
}

int Client::getSocket(void) const
{
    return _socket;
};