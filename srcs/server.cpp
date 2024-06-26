#include "server.hpp"
#include "io_event.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/

server::server(dispatch& d) : _type("server"), _d(d)
{
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1) {
    throw serverException("Error creating server socket");
    }

    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(12345);
    _addr.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    throw serverException("Error setting socket options");
    }

    if (bind(_socket, (struct sockaddr *)&_addr, sizeof _addr) == -1) {
    perror("bind");
    throw serverException("Error binding server socket");
    }

    if (listen(_socket, 5) == -1) {
    perror("listen");
    throw serverException("Error listening on server socket");
    }

    _client_addr_len = sizeof _addr;
};

server::~server(void)
{
    std::cout << "server destructor" << std::endl;
    for (size_t i = 0; i < _clients.size(); ++i)
        delete _clients[i];
};




/****************************************/
/*                METHODS               */
/****************************************/

void server::receive_message(void)
{
    // struct sockaddr_in addr;
    // int addrlen = sizeof addr;
    struct sockaddr_storage remoteaddr;
    socklen_t addrlen = sizeof remoteaddr;

    int sockfd = accept(_socket, (struct sockaddr *)&remoteaddr, &addrlen);

    std::cout << "calling client constructor" << std::endl;
    client *newClient = new client(sockfd, _d);
    _clients.push_back(newClient);
    std::cout << "adding client to dispatch" << std::endl;
    _d.add(*newClient);
}


int server::socket_func(void) const
{
      return _socket;
};



/****************************************/
/*                GETTERS               */
/****************************************/

std::string server::getType(void)
{
    return _type;
}