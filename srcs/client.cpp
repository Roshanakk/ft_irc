#include "client.hpp"
#include "io_event.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/


client::client(int sock_val, dispatch& d) : _socket(sock_val), _type("client"), _d(d) 
{
    if (_socket == -1) {
    throw serverException("Error creating client socket");
    }
}

client::~client(void) {};


/****************************************/
/*                METHODS               */
/****************************************/

void client::send_message(void)
{
    send(_socket, buf, sizeof buf, 0);
}

void client::receive_message(void)
{
    //Receiving client's message
    int numbytes = recv(_socket, buf, sizeof buf, 0);
    if (numbytes < 0)
        throw serverException("Error: failed to receive.");
    else if (numbytes == 0) //If client disconnected
    {
        _d.remove(*this);
        close(_socket);
        return ;
    }
    std::cout << "Received: " << buf << std::endl;
}

int client::socket_func(void) const
{
    return _socket;
};

/****************************************/
/*                GETTERS               */
/****************************************/

std::string client::getType(void)
{
    return _type;
}