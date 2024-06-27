#include "Server.hpp"
#include "ServerReplies.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/

// server::server() {}

Server::Server(Dispatch& d) : _type("server"), _d(d)
{
    //Creating server socket
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1) {
    throw ServerException("Error creating server socket");
    }

    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(12345);
    _addr.sin_addr.s_addr = INADDR_ANY;

    //Allowing socket to be reusable if the program restarts
    //And allowing multiple sockets to bind to the same IP & port
    int opt = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    throw ServerException("Error setting socket options");
    }

    //Preparing server socket for connections
    if (bind(_socket, (struct sockaddr *)&_addr, sizeof _addr) == -1) {
    perror("bind");
    throw ServerException("Error binding server socket");
    }

    if (listen(_socket, 5) == -1) {
    perror("listen");
    throw ServerException("Error listening on server socket");
    }

    _client_addr_len = sizeof _addr;
};

Server::~Server(void)
{
    std::cout << "Server destructor called" << std::endl;
    for (size_t i = 0; i < _clients.size(); ++i)
        delete _clients[i];
    close(_socket);
    close(_d.get_epollfd());
};




/****************************************/
/*                METHODS               */
/****************************************/

void Server::send_message(void) {}

void Server::receive_message(void)
{
    struct sockaddr_storage remoteaddr;
    socklen_t addrlen = sizeof remoteaddr;

    //Client socket fd
    int sockfd = accept(_socket, (struct sockaddr *)&remoteaddr, &addrlen);

    //Creating the client class and adding it to Dispatch '_d'
    Client *newClient = new Client(sockfd, _d);
    _clients.push_back(newClient);
    _d.add(*newClient);
    std::cout << "New client connected: (" << sockfd << ")" << std::endl;
    std::string host = "host";
    std::string nick = "nick";
    std::string prefix = "prefix";
    std::string response = (RPL_WELCOME(host, nick, prefix));
    send(sockfd, response.c_str(), response.size(), 0);
}


int Server::getSocket(void) const
{
    return _socket;
};



/****************************************/
/*                GETTERS               */
/****************************************/

std::string Server::getType(void)
{
    return _type;
}
