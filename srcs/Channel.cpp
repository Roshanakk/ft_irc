#include "Channel.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/

Channel::Channel(std::string name)
{
    //if name doesnt already exist ( check in a <set> of channels ? )
    _name = name;
    //else
    //add '_' at the end of the name
}

Channel::Channel(const Channel & src)
{
    _name = src._name;
}



Channel::~Channel()
{

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