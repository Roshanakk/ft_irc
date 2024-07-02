#include "Command.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/

Command::Command(Client & client)
    : _client(client)
{

    std::string listCmds[21] = {"CAP", "INFO", "INVITE", "JOIN", "LIST", "KICK",
                "KILL", "MODE", "NAMES", "NICK", "NOTICE", 
                "PART", "PING", "PRIMSG", "TOPIC", "USER",
                "VERSION", "WHO", "WHOIS", "WHOWAS", "NB_CMDS" };

    for (size_t i = 0; i < listCmds->length() - 1; ++i)
        _listCmds[i] = listCmds[i];

}

Command::~Command()
{

}




/*****************************************/
/*                COMMANDS               */
/*****************************************/

// void Commands::doNICK(std::string & nickname, AUser * user)
// {
//     user->setNickname(nickname);
// }

// static int isCmd(std::string line)
// {

// }

int Command::whatCmd(std::string & line)
{
    std::string::size_type spacePos = line.find(' ');
    std::string firstWord = line.substr(0, spacePos);

    // for (size_t i = 0; i < _listCmds->length() - 1; ++i)
    //     std::cout << _listCmds[i] << std::endl;

    std::cout << "_listCmds length" << _listCmds->length() << std::endl;

    for (int i = 0; i < NB_CMDS; ++i)
    {
        if (firstWord == _listCmds[i])
            return (i);
    }

    (void) _client;
    return (-1);
}

// void Command::doCmd(std::string & line)
// {

//     try
//     {
//         int cmd = whatCmd(line);
//         if (cmd < 0)
//         {
//             // throw(NoCommandException())
//         }
//         /* code */
//     }
//     catch(const std::exception& e)
//     {
//         std::cerr << e.what() << '\n';
//     }
    

    
//     std::string listCmds[3] = {"NICK", "USER", "EXIT"};
        
// }