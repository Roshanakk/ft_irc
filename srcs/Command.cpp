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

    _cmd = line.substr(0, firstSpacePos);
    _parameters = line.substr(firstSpacePos + 1);

    _parameters.erase(std::remove(_parameters.begin(), _parameters.end(), '\n'), _parameters.end());
    _parameters.erase(std::remove(_parameters.begin(), _parameters.end(), '\r'), _parameters.end());

    try
    {
        int i = 0;
        while (i < NB_CMDS && _cmd != _listCmds[i])
            ++i;

        if (i == NB_CMDS)
            throw(NoCommandException(ERR_UNKNOWNCOMMAND(_client.getHostname(), _cmd)));
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
    if (_parameters.size() <= 1)
        throw NoCommandException(ERR_NEEDMOREPARAMS(_cmd));

    // Split parameters by space
    std::vector<std::string> params = Utilities::split(_parameters, ' ');

    // Print parameters
    for (size_t i = 0; i < params.size(); ++i)
    {
        std::cout << "JOIN: '" << params[i] << "'" << std::endl;
    }
    std::cout << "Number of parameters: " << params.size() << std::endl;

    ChannelManager& cm = _client.getCM();
    Channel *chan = cm.getChannel(params[0]);
    if (chan != NULL) {
        // channel exists. start channel checks
        std::cout << "Channel exists" << std::endl;
        // check that user is not a member of too many channels
            // just search for the user in all the channels? not fast but would be easy to implement.
        // check that user is not banned from channel
            // Add a banned list to the channel class
        // check that the channel is not invite only
            // Add flags to the channel class
        // check that the channel is not full
            // Add a max number of users to the channel class.
            // I think this is infinite at first, but can be chagned with mode +l #
            // Add a checkMax function to the channel class
        // check that the channel mask is good ??? (unsure what this means)
            // idk what this is yet.

        // check password
        if (chan->requiresKey()) {
            // Channel requires a password
                // NOTE: no spaces are allowed in the password so we can just chck params[1]
            if (params.size() > 1 && chan->checkKey(params[1])) {
                std::cout << "Password is correct" << std::endl;
                chan->addClient(&_client);
                // Response to send: RPL_TOPIC and RPL_NAMREPLY
            } else {
                std::cout << "Password is incorrect" << std::endl;
                throw NoCommandException(ERR_PASSWDMISMATCH());
            }
        } else {
            // Channel does not require a password
            chan->addClient(&_client);
            // Response to send: RPL_TOPIC and RPL_NAMREPLY
        }
    } else {
        // Channel does not exist. Create channel and add client to channel.
        std::cout << "Channel does not exist, creating channel" << std::endl;
        cm.addChannel(params[0], &_client);
        // Response to send: RPL_TOPIC and RPL_NAMREPLY
    }
    std::cout << "Number of channels: " << cm.getNumChannels() << std::endl;
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
    // // Initial error checking
    // if (_cmd.size() != 2) {
    //     if (_cmd.size() == 1)
    //         throw NoCommandException(ERR_NORECIPIENT(_cmd));
    //     else
    //         throw NoCommandException(ERR_NOTEXTTOSEND());
    // }

    // // Split parameters by space
    // // std::vector<std::string> params = Utilities::split(_cmdLine[1], ' ');

    // size_t firstSpacePos = _parameters.find(' ');
    // if (firstSpacePos == std::string::npos)
    //     firstSpacePos = _parameters.size() - 1;

    // std::string recipeints = _parameters.substr(0, firstSpacePos);
    // std::string message = _parameters.substr(firstSpacePos + 1);


    // // Print parameters
    // // for (size_t i = 0; i < params.size(); ++i)
    // // {
    // //     std::cout << "PRIVMSG: '" << params[i] << "'" << std::endl;
    // // }

    // // Check if the first parameter is a channel or a user
    // if (recipeints[0] == '#') {
    //     // Channel
    //     ChannelManager& cm = _client.getCM();
    //     Channel *chan = cm.getChannel(recipeints);
    //     if (chan != NULL) {
    //         // Channel exists
    //         std::cout << "Channel exists" << std::endl;
    //         // Send message to all clients in channel
    //         chan->forwardMessage(message, &_client);
    //     } else {
    //         // Channel does not exist
    //         std::cout << "Channel does not exist" << std::endl;
    //         throw NoCommandException(ERR_CANNOTSENDTOCHAN(recipeints));
    //     }
    // } else {
    //     // User
    //     // Send message to user
    //     // _client.forwardMessage(_cmdLine[1], &_client);
    // }
}

void Command::handle_TOPIC() {}
void Command::handle_USER() {}
void Command::handle_VERSION() {}
void Command::handle_WHO() {}
void Command::handle_WHOIS() {}
void Command::handle_WHOWAS() {}