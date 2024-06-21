#include "UserRegular.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/

UserRegular::UserRegular(std::string nickname) : AUser(nickname)
{

}

UserRegular::UserRegular(const UserRegular & src) : AUser(src)
{
    _isOperator = src._isOperator;
    _nickname = src._nickname;

}



UserRegular::~UserRegular()
{

}


/**********************************************************/
/*                   OPERATORS OVERLOAD                   */
/**********************************************************/

UserRegular & UserRegular::operator=(const UserRegular & src)
{
    if (this != &src)
        *this = src;
    return (*this);
}