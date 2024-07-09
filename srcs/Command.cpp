#include "Command.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/

Command::Command(Client & client)
    : _client(client)
{

    std::string listCmds[21] = {"CAP", "INFO", "INVITE", "JOIN", "LIST", "KICK",
                "KILL", "MODE", "NAMES", "NICK", "NOTICE", 
                "PART", "PING", "PRIVMSG", "TOPIC", "USER",
                "VERSION", "WHO", "WHOIS", "WHOWAS", "NB_CMDS" };

    void  (Command::*fctCmds[21])() = {&Command::handle_CAP, &Command::handle_INFO, &Command::handle_INVITE,
                                &Command::handle_JOIN, &Command::handle_LIST, &Command::handle_KICK, &Command::handle_KILL, &Command::handle_MODE,
                                &Command::handle_NAMES, &Command::handle_NICK, &Command::handle_NOTICE, &Command::handle_PART,
                                &Command::handle_PING, &Command::handle_PRIVMSG, &Command::handle_TOPIC, &Command::handle_USER,
                                &Command::handle_VERSION, &Command::handle_WHO, &Command::handle_WHOIS, &Command::handle_WHOWAS};

    for (size_t i = 0; i < NB_CMDS; ++i)
    {
        _listCmds[i] = listCmds[i];
        _fctCmds[i] = fctCmds[i];
    }

}

Command::~Command()
{

}




/*************************************************/
/*                GENERAL MANAGING               */
/*************************************************/

// void Commands::doNICK(std::string & nickname, AUser * user)
// {
//     user->setNickname(nickname);
// }

// static int isCmd(std::string line)
// {

// }

// int Command::whatCmd(std::string & line)
// {
//     std::string::size_type spacePos = line.find(' ');
//     std::string firstWord = line.substr(0, spacePos);

//     for (int i = 0; i < NB_CMDS; ++i)
//     {
//         if (firstWord == _listCmds[i])
//             return (i);
//     }

//     (void) _client;
//     return (-1);
// }

void Command::doCmd(std::string & line)
{
    size_t firstSpacePos = line.find(' ');
    if (firstSpacePos == std::string::npos)
        firstSpacePos = line.size() - 1;

    std::string cmd = line.substr(0, firstSpacePos);
    std::string parameters = line.substr(firstSpacePos);
    parameters.erase(std::remove(parameters.begin(), parameters.end(), '\n'));
    parameters.erase(0, 1);

    _cmdLine.push_back(cmd);
    _cmdLine.push_back(parameters);

    try
    {
        int i = 0;
        while (i < NB_CMDS && _cmdLine[0] != _listCmds[i])
            ++i;

        if (i == NB_CMDS)
            throw(CommandException(ERR_UNKNOWNCOMMAND(_client.getHostname(), _cmdLine[0])));
        else
        {
            // std::cout << "CMD = " << i << std::endl;
            // std::cout << "_cmdLine[1] = " << _cmdLine[1] << std::endl;

            (this->*(_fctCmds[i]))();
        }
    }
    catch(const std::exception& e)
    {
        _client.send_message(e.what());
    }
    
}


/*****************************************/
/*                COMMANDS               */
/*****************************************/

void Command::handle_CAP() 
{
    // std::cout << "Cap command" << std::endl;
}


void Command::handle_INFO() {}
void Command::handle_INVITE() {}

void Command::handle_JOIN() {

    // std::cout << "YOUHOU" << std::endl;
}

void Command::handle_LIST() {}
void Command::handle_KICK() {}
void Command::handle_KILL() {}
void Command::handle_MODE() {}
void Command::handle_NAMES() {}



bool isAValidNickname(std::string str)
{
    std::string set = "|^_-{}[]";

    for (size_t i = 0; i < str.size(); ++i)
    {
        if (!std::isalnum(str[i]) && set.find_first_of(str[i]) == std::string::npos)
        {
            std::cout << str[i] << " -> NOPE" << std::endl;
            return (false);
        }
    }
    return (true);
}


void Command::handle_NICK() 
{

    std::vector<std::string> params = Utilities::split(_cmdLine[1], ' ');

    if (params.size() < 1)
        throw(CommandException(ERR_NONICKNAMEGIVEN()));
    else if (params.size() == 1)
    {
        if (!params[0].empty())
            params[0].erase(params[0].size() - 1); //removing the trailing \r 
        if (!isAValidNickname(params[0]))
            throw(CommandException(ERR_ERRONEUSNICKNAME(params[0])));

        // //if nickname already exists
        // if ()
    }
    else
        return ; // no error if too many parameters ?
    
}




void Command::handle_NOTICE() {}
void Command::handle_PART() {}

void Command::handle_PING()
{
    std::string pong = "PONG ";

    send(_client.getSocket(), pong.c_str(), pong.size(), 0);
}

void Command::handle_PRIVMSG() {}
void Command::handle_TOPIC() {}



void Command::handle_USER()
{
    //Splitting the parameters string into a vector of strings,
    //in order to extract username, hostname and realname

    size_t colonPos = _cmdLine[1].find(':');
    std::string tmpParams = _cmdLine[1].substr(0, colonPos);

    std::vector<std::string> params;
    params = Utilities::split(tmpParams, ' ');
    if (params.size() > 0)
        params.push_back(_cmdLine[1].substr(colonPos + 1));

    //Setting username, hostname, realname
    if (params.size() == 1)
    {
        _client.setUsername(params[0]);
        _client.setHostname("localhost");
        _client.setRealname("localhost");        
    }
    else if (params.size() >= 4)
    {
        _client.setUsername(params[0]);
        _client.setHostname(params[2]);
        _client.setRealname(params[3]);
    }
    else
        throw(CommandException(ERR_NEEDMOREPARAMS(_cmdLine[0])));

}




void Command::handle_VERSION() {}
void Command::handle_WHO() {}
void Command::handle_WHOIS() {}
void Command::handle_WHOWAS() {}