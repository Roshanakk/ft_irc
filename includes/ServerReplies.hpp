
//Format not defined yet. Macros implemented only for tests for now

#define RPL_WELCOME(hostname, nick, prefix) (":" + hostname + " 001 " + nick + " :Welcome to the Internet Relay Network " + prefix + "\r\n")

//NICK
#define ERR_NONICKNAMEGIVEN() (":No nickname given\r\n")


#define ERR_ERRONEUSNICKNAME(nickname) (nickname + " :Erroneous nickname")

//Returned after receiving a NICK message which contains
        //    characters which do not fall in the defined set.  See
        //    section 2.3.1 for details on valid nicknames.

//USER
#define ERR_UNKNOWNCOMMAND(hostname, cmd) (":" + hostname + " 421 " + cmd + " :Unknown command\r\n") 
#define ERR_NEEDMOREPARAMS(cmd) (" 461 " + cmd + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED() (":Unauthorized command (already registered)\r\n")


