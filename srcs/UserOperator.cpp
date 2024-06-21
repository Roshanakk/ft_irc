#include "UserOperator.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/

UserOperator::UserOperator(std::string nickname) : AUser(nickname)
{

}

UserOperator::UserOperator(const UserOperator & src)
{
    _isOperator = src._isOperator;
    _nickname = src._nickname;
}



UserOperator::~UserOperator()
{

}


/**********************************************************/
/*                   OPERATORS OVERLOAD                   */
/**********************************************************/

UserOperator & UserOperator::operator=(const UserOperator & src)
{
    if (this != &src)
        *this = src;
    return (*this);
}
