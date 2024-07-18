#include "Command.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/

Command::Command(Client & client)
    : _client(client)
{

    std::string listCmds[23] = {"CAP", "INFO", "INVITE", "JOIN", "LIST", "KICK",
                "KILL", "MODE", "NAMES", "NICK", "NOTICE", 
                "PART", "PASS", "PING", "PRIVMSG", "QUIT", "TOPIC", "USER",
                "VERSION", "WHO", "WHOIS", "WHOWAS", "NB_CMDS" };

    void  (Command::*fctCmds[23])() = {&Command::handle_CAP, &Command::handle_INFO, &Command::handle_INVITE,
                                &Command::handle_JOIN, &Command::handle_LIST, &Command::handle_KICK, &Command::handle_KILL, 
                                &Command::handle_MODE, &Command::handle_NAMES, &Command::handle_NICK, &Command::handle_NOTICE, 
                                &Command::handle_PART, &Command::handle_PASS, &Command::handle_PING, &Command::handle_PRIVMSG, 
                                &Command::handle_QUIT, &Command::handle_TOPIC, &Command::handle_USER,
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
            throw(CommandException(ERR_UNKNOWNCOMMAND(_client.getHostname(), _cmd)));
        if (_client.getStatus() != PASS_REGISTERED && _cmd != "PASS" && _cmd != "NICK" && _cmd != "USER") {
            if (_client.getStatus() == PASS_NEEDED)
                throw(CommandException(ERR_PASSWDNEEDED()));
            else
                throw(CommandException(ERR_NOTREGISTERED()));
        }
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

void Command::handle_CAP() 
{
    // std::cout << "Cap command" << std::endl;
}


void Command::handle_INFO() {}
void Command::handle_INVITE() {}

void Command::handle_JOIN() {
    // Need to check if there are parameters. If not, throw an exception.
    if (_parameters.size() <= 1)
        throw CommandException(ERR_NEEDMOREPARAMS(_cmd));

    // Split parameters by space
    std::vector<std::string> paramsVec = Utilities::split(_parameters, ' ');

    std::vector<std::string> channelsVec = Utilities::split(paramsVec[0], ',');
    std::vector<std::string> passwordsVec;
    if (paramsVec.size() > 1)
        passwordsVec = Utilities::split(paramsVec[1], ',');
    ChannelManager& cm = _client.getCM();

    for (size_t i = 0; i < channelsVec.size(); ++i) {
        Channel *chan = cm.getChannel(channelsVec[i]);

        // Check that user is not in too many channels
        if (cm.getClientChannelCount(&_client) >= cm.getMaxChannelsForClient()) {
            throw CommandException(ERR_TOOMANYCHANNELS(channelsVec[i]));
        }

        // Following checks are only required if the channel exists
        if (chan != NULL) {
            // channel exists. start channel checks
            std::cout << "Channel exists" << std::endl;
            // check that user is not banned from channel
                // This is a check in the protocol but not required by the subject.
            // check that the channel is not invite only
            if (chan->getInviteOnly()) {
                if (chan->checkInvite(&_client)) {
                    std::cout << "User is invited" << std::endl;
                } else {
                    std::cout << "User is not invited" << std::endl;
                    throw CommandException(ERR_INVITEONLYCHAN());
                }
            }
            // check that the channel is not full
            if (!chan->checkCanAddMoreClients()) {
                std::cout << "Channel is full" << std::endl;
                throw CommandException(ERR_CHANNELISFULL());
            }
            // check that the channel mask is good
                // This is in the protocol for ops only. Not required by the subject.
            // check password
            if (chan->requiresKey()) {
                // Channel requires a password
                    // NOTE: no spaces are allowed in the password so we can just chck channelsVec[1]
                if (paramsVec.size() > 1 && chan->checkKey(passwordsVec[i])) {
                    std::cout << "Password is correct" << std::endl;
                    chan->addClient(&_client);
                } else {
                    std::cout << "Password is incorrect" << std::endl;
                    throw CommandException(ERR_PASSWDMISMATCH());
                }
            } else {
                // Channel does not require a password
                chan->addClient(&_client);
            }
        } else {
            // Channel does not exist. Create channel and add client to channel.
            std::cout << "Channel does not exist, creating channel" << std::endl;
            cm.addChannel(channelsVec[i], &_client);
        }
        std::cout << "Number of channels: " << cm.getNumChannels() << std::endl;
    }
}

void Command::handle_LIST() {}
void Command::handle_KICK() {}
void Command::handle_KILL() {}
void Command::handle_MODE() {
    // NOTE: Normally the mode command can be used on users, but in our implementation and according to the subject,
    // we only need to implement it for channels.
    //
    // Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>]
    // MODE - Change the channel’s mode:
    //   i: Set/remove Invite-only channel
    //   t: Set/remove the restrictions of the TOPIC command to channel operators
    //   k: Set/remove the channel key (password)
    //   o: Give/take channel operator privilege
    //   l: Set/remove the user limit to channel

    // Need to check if there are parameters. If not, throw an exception.
    if (_parameters.size() <= 1)
        throw CommandException(ERR_NEEDMOREPARAMS(_cmd));

    // basic parsing so get the channel or user we're modifying.
        // after split there should be at least 2 elements. 
        // First should be the channel or user, and the second should be the mode.
    std::vector<std::string> paramsVec = Utilities::split(_parameters, ' ');
    if (paramsVec.size() < 2)
        throw CommandException(ERR_NEEDMOREPARAMS(_cmd));
    if (paramsVec[0][0] != '#')
        throw CommandException(ERR_NOSUCHCHANNEL(paramsVec[0]));

    // Check that the client is in the channel and is a channel operator for this channel
    Channel *chan = _client.getCM().getChannel(paramsVec[0]);
    if (chan == NULL)
        throw CommandException(ERR_NOSUCHCHANNEL(paramsVec[0]));
    if (!chan->checkIfClientInChannel(&_client))
        throw CommandException(ERR_NOTONCHANNEL(paramsVec[0]));
    if (!chan->checkIfClientIsOp(&_client))
        throw CommandException(ERR_CHANOPRIVSNEEDED(paramsVec[0]));

    // Check the mode and perform the appropriate action
    // come in the form of +il-k for example. The positives first as a group, then the negatives
    // first check if the first character is a + or a -
    // then check if the character is in our list of commands (freenode throws an error if it's not)
    // Do each command, while checking if there is a - because then we will switch the command mode
    // finish the commands in the same manner as the positives
    bool positive = true;
    size_t argNum = 2;
    for (size_t i = 0; i < paramsVec[1].size(); ++i) {
        switch (paramsVec[1][i]) {
            case '+':
                positive = true;
                break;
            case '-':
                positive = false;
                break;
            case 'i':
                handle_MODE_i(positive, chan);
                break;
            case 't':
                handle_MODE_t(positive, chan);
                break;
            case 'k':
                handle_MODE_k(positive, chan,
                    (positive && argNum < paramsVec.size() 
                    ? paramsVec[argNum++] : "") );
                break;
            case 'o':
                handle_MODE_o(positive, chan,
                    (argNum < paramsVec.size() 
                    ? paramsVec[argNum++] : "") );
                break;
            case 'l':
                handle_MODE_l(positive, chan,
                    (positive && argNum < paramsVec.size() 
                    ? paramsVec[argNum++] : "") );
                break;
            default:
                // inform client that the mode is not recognized
                _client.send_message(ERR_UNKNOWNMODE(_client.getNickname(), paramsVec[1][i]));
                break;
        }
    }
}

void Command::handle_NAMES() {}



bool isAValidNickname(std::string str)
{
    std::string set = "|^_-{}[]";

    if (std::isdigit(str[0]) || str[0] == '-')
        return (false);
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
    std::vector<std::string> params = Utilities::split(_parameters, ' ');
    std::string oldNickname;

    //if user mode is +r
    //throw(CommandException(ERR_RESTRICTED()))

    if (_client.getStatus() == PASS_NEEDED)
        throw(CommandException(ERR_PASSWDNEEDED()));

    if (_client.getNickname().empty() && params.size() < 1)
        throw(CommandException(ERR_NONICKNAMEGIVEN()));
    else if (params.size() < 1)
        _client.send_message(_client.getNickname() + "\r\n");
    else if (params.size() == 1)
    {
        std::string nickname = params[0];
        if (!isAValidNickname(nickname))
            throw(CommandException(ERR_ERRONEUSNICKNAME(nickname)));

        std::set<Client *> & allClients = _client.getClients();
        std::set<Client *>::iterator it = allClients.begin();

        //Checking if the nickname is already used by another user
        while (it != allClients.end() && nickname != (*it)->getNickname())
            ++it;

        if (it != allClients.end())
        {
            if (_client.getNickname() == "default") // if it is the first connection (first NICK call)
                _client.setNickname(nickname + '_');
            else                                    // if the user tries to change his nickname (user already connected)
                throw(CommandException(ERR_NICKNAMEINUSE(nickname)));
        }
        else
        {
            oldNickname = _client.getNickname();
            _client.setNickname(nickname);
        }
        
    }

    if (!_client.isAuth())
    {
        _client.setNickAuth();
        if (_client.isAuth())
        {
            _client.setStatus(PASS_REGISTERED);
            _client.send_message(RPL_WELCOME(_client.getHostname(), _client.getNickname(), _client.getPrefix()));
        }
    }

    _client.send_message(RPL_NICK(_client.getPrefix(), _client.getNickname()));

}


void Command::handle_NOTICE() {}
void Command::handle_PART() {
    if (_parameters.size() <= 1)
        throw CommandException(ERR_NEEDMOREPARAMS(_cmd));

    std::vector<std::string> params = Utilities::split(_parameters, ' ');

    ChannelManager& cm = _client.getCM();
    Channel *chan = cm.getChannel(params[0]);

    if (chan != NULL) {
        if (chan->checkIfClientInChannel(&_client)) {
            chan->removeClient(&_client);
        } else {
            throw CommandException(ERR_NOTONCHANNEL(params[0]));
        }
    } else {
        throw CommandException(ERR_NOSUCHCHANNEL(params[0]));
    }
    cm.removeEmptyChannels();
}


void Command::handle_PASS()
{
    if (_client.isAuth())
        throw(CommandException(ERR_ALREADYREGISTRED()));

    std::vector<std::string> params = Utilities::split(_parameters, ' ');

    if (params.size() < 1)
        throw(CommandException(ERR_NEEDMOREPARAMS(_cmd)));
    
    std::string password = params[0];

    if (password == _client.getDispatch().getPassword())
    {
        _client.setPassAuth();
        _client.setStatus(PASS_CORRECT);
    }
    else
        throw(CommandException(ERR_PASSWDMISMATCH()));

    if (!_client.isAuth())
    {
        _client.setPassAuth();
        if (_client.isAuth())
            _client.send_message(RPL_WELCOME(_client.getHostname(), _client.getNickname(), _client.getPrefix()));
    }


}

void Command::handle_PING()
{
    std::string pong = "PONG ft_irc\r\n";
    _client.send_message(pong);
}

void Command::handle_PRIVMSG() {
    // Initial error checking
    if (_parameters.size() <= 1)
        throw CommandException(ERR_NEEDMOREPARAMS(_cmd));

    size_t firstSpacePos = _parameters.find(' ');
    if (firstSpacePos == std::string::npos)
        firstSpacePos = _parameters.size() - 1;

    std::string recipeints = _parameters.substr(0, firstSpacePos);
    std::string message = _parameters.substr(firstSpacePos + 1);

    if (message[0] == ':')
        message = message.substr(1);

    // Check if the first parameter is a channel or a user
    if (recipeints[0] == '#') {
        // Channel
        ChannelManager& cm = _client.getCM();
        Channel *chan = cm.getChannel(recipeints);

        if (chan != NULL) {
            // Channel exists
            std::cout << "Channel exists" << std::endl;
            if (chan->checkIfClientInChannel(&_client)) {
                chan->forwardMessage(message, &_client);
            } else {
                throw CommandException(ERR_CANNOTSENDTOCHAN(recipeints));
            }
        } else {
            // Channel does not exist
            std::cout << "Channel does not exist" << std::endl;
            throw CommandException(ERR_CANNOTSENDTOCHAN(recipeints));
        }
    } else {
        // User
        // Send message to user
        // _client.forwardMessage(_cmdLine[1], &_client);
    }
}

void Command::handle_QUIT() {
    // this sets the shouldDelete flag to true which is actually deleted 
    // in the dispatch run command.
    std::cout << "Client quitting" << std::endl;
    _client.setShouldDelete(true);
}

void Command::handle_TOPIC() {}



void Command::handle_USER()
{
    //Splitting the parameters string into a vector of strings,
    //in order to extract username, hostname and realname

    if (_client.isAuth())
        throw(CommandException(ERR_ALREADYREGISTRED()));

    size_t colonPos = _parameters.find(':');
    std::string tmpParams = _parameters.substr(0, colonPos);

    std::vector<std::string> params;
    params = Utilities::split(tmpParams, ' ');
    if (params.size() > 0)
        params.push_back(_parameters.substr(colonPos + 1));

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
        _client.setHostname(params[1]);
        _client.setRealname(params[3]);
    }
    else
        throw(CommandException(ERR_NEEDMOREPARAMS(_cmd)));

    if (!_client.isAuth())
    {
        _client.setUserAuth();
        if (_client.isAuth())
        {
            _client.setStatus(PASS_REGISTERED);
            _client.send_message(RPL_WELCOME(_client.getHostname(), _client.getNickname(), _client.getPrefix()));
        }
    }


}




void Command::handle_VERSION() {}
void Command::handle_WHO() {}
void Command::handle_WHOIS() {}
void Command::handle_WHOWAS() {}


// Mode flags

void Command::handle_MODE_i(bool posFlag, Channel *chan) {
    std::cout << "Mode " << (posFlag ? "+" : "-") 
              << "i" << std::endl;
    if (chan->getInviteOnly() && posFlag) {
        throw CommandException(ERR_INVITEONLYCHAN());
    }
    (void)chan;
};

void Command::handle_MODE_t(bool posFlag, Channel *chan) {
    std::cout << "Mode " << (posFlag ? "+" : "-") 
              << "t" << std::endl;
    (void)chan;
};

void Command::handle_MODE_k(bool posFlag, Channel *chan, std::string arg) {
    std::cout << "Mode " << (posFlag ? "+" : "-") 
              << "k " << arg << std::endl;
    (void)chan;
};

void Command::handle_MODE_o(bool posFlag, Channel *chan, std::string arg) {
    std::cout << "Mode " << (posFlag ? "+" : "-") 
              << "o " << arg << std::endl;
    (void)chan;
};

void Command::handle_MODE_l(bool posFlag, Channel *chan, std::string arg) {
    std::cout << "Mode " << (posFlag ? "+" : "-") 
              << "l " << arg << std::endl;
    (void)chan;
};
