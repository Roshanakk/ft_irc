#include "Server.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/

// server::server() {}


Server::Server(Dispatch& d, int port, 
    std::set<Client *> & clients, ChannelManager & cm)
    : _port(port), _d(d), _clients(clients), _cm(cm), _shouldDelete(false)
{
    //Creating server socket
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1) {
        throw ServerException("Error creating server socket");
    }

    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(_port);
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
    for (std::set<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
        delete *it;
    close(_socket);
    close(_d.get_epollfd());
};




/****************************************/
/*                METHODS               */
/****************************************/

void Server::send_message(std::string message) {
    (void) message;
}

void Server::receive_message(void)
{
    struct sockaddr_storage remoteaddr;
    socklen_t addrlen = sizeof remoteaddr;

    //Client socket fd
    int sockfd = accept(_socket, (struct sockaddr *)&remoteaddr, &addrlen);

    //Creating the client class and adding it to Dispatch '_d'
    Client *newClient = new Client(sockfd, _d, _clients, _cm);
    _clients.insert(newClient);
    _d.add(*newClient);
    std::cout << "New client connected: (" << sockfd << ")" << std::endl;

    // Need to accept the conection but do not give the client full 
    // access to the server until we verify all information and the 
    // client gives the correcct password.

    // This portion will need to be sent in addition to rpl 002 003 and 004
    // Will not be in this function; should be in 
    // std::string host = "host";
    // std::string nick = "nick";
    // std::string prefix = "prefix";

    
    // std::string response = (RPL_WELCOME(host, nick, prefix));
    // send(sockfd, response.c_str(), response.size(), 0);
}


int Server::getSocket(void) const {
    return _socket;
};

bool Server::shouldDelete(void) const {
    return (_shouldDelete);
};