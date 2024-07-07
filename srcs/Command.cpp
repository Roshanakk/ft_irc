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
    std::string parameters = line.substr(firstSpacePos + 1);

    parameters.erase(std::remove(parameters.begin(), parameters.end(), '\n'), parameters.end());
    parameters.erase(std::remove(parameters.begin(), parameters.end(), '\r'), parameters.end());

    if (cmd.size() > 0 ) {
        _cmdLine.push_back(cmd);
    }

    if (parameters.size() > 0 ) {
        _cmdLine.push_back(parameters);
    }

    try
    {
        int i = 0;
        while (i < NB_CMDS && _cmdLine[0] != _listCmds[i])
            ++i;

        if (i == NB_CMDS)
            throw(NoCommandException(ERR_UNKNOWNCOMMAND(_client.getHostname(), _cmdLine[0])));
        else
            (this->*(_fctCmds[i]))();
    }
    catch(const std::exception& e)
    {
        _client.send_message(e.what());
    }
    
}


/*****************************************/
/*                COMMANDS               */
/*****************************************/

void Command::handle_CAP() {}
void Command::handle_INFO() {}
void Command::handle_INVITE() {}

void Command::handle_JOIN() {
    // Need to check if there are parameters. If not, throw an exception.
    if (_cmdLine.size() <= 1)
        throw NoCommandException(ERR_NEEDMOREPARAMS(_cmdLine[0]));

    // Split parameters by space
    std::vector<std::string> params = Utilities::split(_cmdLine[1], ' ');

    // Print parameters
    for (size_t i = 0; i < params.size(); ++i)
    {
        std::cout << "JOIN: '" << params[i] << "'" << std::endl;
    }

    std::string chanName = params[0];
    std::string chanKey = params.size() > 1 ? params[1] : "";

    ChannelManager& cm = _client.getCM();
    Channel *chan = cm.getChannel(chanName);
    if (chan != NULL) {
        // channel exists. check password
        std::cout << "Channel exists" << std::endl;
        if (chan->checkKey(chanKey)) {
            std::cout << "Password is correct" << std::endl;
            chan->addClient(&_client);
            // Response to send: RPL_TOPIC and RPL_NAMREPLY
        } else {
            std::cout << "Password is incorrect" << std::endl;
            throw NoCommandException(ERR_PASSWDMISMATCH());
        }
    } else {
        // Channel does not exist. Create channel and add client.
        std::cout << "Channel does not exist, creating channel" << std::endl;
        cm.addChannel(chanName, &_client, chanKey);
        // Response to send: RPL_TOPIC and RPL_NAMREPLY
    }
    //std::cout << "Number of channels: " << cm.getNumChannels() << std::endl;
}

void Command::handle_LIST() {}
void Command::handle_KICK() {}
void Command::handle_KILL() {}
void Command::handle_MODE() {}
void Command::handle_NAMES() {}
void Command::handle_NICK() {}
void Command::handle_NOTICE() {}
void Command::handle_PART() {}

void Command::handle_PING()
{
    std::string pong = "PONG ";

    send(_client.getSocket(), pong.c_str(), pong.size(), 0);
}

void Command::handle_PRIVMSG() {
    // Split parameters by space
    std::vector<std::string> params = Utilities::split(_cmdLine[1], ' ');

    // Print parameters
    for (size_t i = 0; i < params.size(); ++i)
    {
        std::cout << "PRIVMSG: '" << params[i] << "'" << std::endl;
    }

    // (*it)->forwardMessage(_cmdLine[1], &_client);
}

void Command::handle_TOPIC() {}
void Command::handle_USER() {}
void Command::handle_VERSION() {}
void Command::handle_WHO() {}
void Command::handle_WHOIS() {}
void Command::handle_WHOWAS() {}