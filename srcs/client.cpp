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
