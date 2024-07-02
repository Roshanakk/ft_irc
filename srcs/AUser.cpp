#include "AUser.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/

AUser::AUser() : _isOperator(0)
{
    //if "default" doesnt already exist ( check in a <set> of channels ? )
    _nickname = "default";
    //else
    //add '_' at the end of the name
}

AUser::AUser(std::string nickname) : _isOperator(0)
{
    //if name doesnt already exist ( check in a <set> of channels ? )
    _nickname = nickname;
    //else
    //add '_' at the end of the name
}

AUser::AUser(const AUser & src)
{
    _isOperator = src._isOperator;
}



AUser::~AUser()
{

}

/**********************************************************/
/*                   OPERATORS OVERLOAD                   */
/**********************************************************/

AUser & AUser::operator=(const AUser & src)
{
    if (this != &src)
        *this = src;
    return (*this);
}

/*********************************************************/
/*                 GETTERS & SETTERS                     */
/*********************************************************/

std::string AUser::getNickname() const
{
    return (_nickname);
}

void AUser::setNickname(std::string nickname)
{
    _nickname = nickname;
}
