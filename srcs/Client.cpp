#include "Client.hpp"
#include "IO_Event.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/


Client::Client(int sock_val, Dispatch& d) : _socket(sock_val), _type("client"), _d(d) 
{
    if (_socket == -1) {
    throw ServerException("Error creating client socket");
    }
}

Client::~Client(void) {};


/****************************************/
/*                METHODS               */
/****************************************/

void Client::send_message(void)
{
    send(_socket, buf, sizeof buf, 0);
}

void Client::receive_message(void)
{
    //Receiving client's message
    int numbytes = recv(_socket, buf, sizeof buf, 0);
    if (numbytes < 0)
        throw ServerException("Error: failed to receive.");
    else if (numbytes == 0) //If client disconnected
    {
        _d.remove(*this);
        close(_socket);
        return ;
    }
    std::cout << "Received: " << buf << std::endl;
    std::string response = "Message received\n";
    send(_socket, response.c_str(), response.size(), 0);
    throw ServerException("Error: failed to receive.");
}

int Client::socket_func(void) const
{
    return _socket;
};

/****************************************/
/*                GETTERS               */
/****************************************/

std::string Client::getType(void)
{
    return _type;
}