#include "Command.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/

Command::Command(Client & client, std::set<Client *>& clients)
	: _client(client), _clients(clients)
{

    std::string listCmds[23] = {"CAP", "INFO", "INVITE", "JOIN", "LIST", "KICK",
                "KILL", "MODE", "NAMES", "NICK", "NOTICE", 
                "PART", "PASS", "PING", "PRIVMSG", "QUIT", "TOPIC", "USER",
                "version", "WHO", "WHOIS", "WHOWAS", "NB_CMDS" };

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

void Command::doCmd(std::string & line)
{
	size_t firstSpacePos = line.find(' ');
	if (firstSpacePos == std::string::npos)
		firstSpacePos = line.size() - 1;

	_cmd = line.substr(0, firstSpacePos);
	_parameters = line.substr(firstSpacePos + 1);

	_parameters.erase(std::remove(_parameters.begin(), _parameters.end(), '\n'), _parameters.end());
	_parameters.erase(std::remove(_parameters.begin(), _parameters.end(), '\r'), _parameters.end());
	_cmd.erase(std::remove(_cmd.begin(), _cmd.end(), '\r'), _cmd.end());

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

void Command::handle_CAP() {}

// void Command::handle_INFO() {}

void Command::handle_INVITE()
{
	// Parameters: <nickname> <channel>

	if (_parameters.empty())
		throw(CommandException(ERR_NEEDMOREPARAMS(_cmd)));
	
	int spacePos = _parameters.find(' ');
	std::string userToInviteName = _parameters.substr(0, spacePos);
	
	Client * userToInvite = getMatchingClient(userToInviteName);

	if (userToInvite == NULL)
		throw(CommandException(ERR_NOSUCHNICK(userToInviteName)));
	
	std::string channelName = _parameters.substr(spacePos + 1);

	if (channelName.empty())
		throw(CommandException(ERR_NEEDMOREPARAMS(_cmd)));
	
	
	ChannelManager & cm = _client.getCM();
	std::set<Channel *> channels = cm.getChannels();

	Channel * channel = getMatchingChannel(channelName, channels); 

	if (channel == NULL)
		throw(CommandException(ERR_NOSUCHCHANNEL(_client.getNickname(), channelName)));
	
	if (!channel->checkIfClientInChannel(&_client))
		throw(CommandException(ERR_NOTONCHANNEL(channelName)));
	
	if (!channel->checkIfClientOperator(&_client))
		throw(CommandException(ERR_CHANOPRIVSNEEDED(_client.getPrefix(), channelName)));
	
	if (channel->checkIfClientInChannel(userToInvite))
		throw(CommandException(ERR_USERONCHANNEL(userToInviteName, channelName)));

	_client.send_message(RPL_INVITING(_client.getNickname(), userToInviteName, channelName));
	userToInvite->send_message(RPL_INVITE(_client.getPrefix(), userToInviteName, channelName));

}

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
                    throw CommandException(ERR_INVITEONLYCHAN(_client.getNickname(), chan->getName()));
                }
            }
            // check that the channel is not full
            if (!chan->checkCanAddMoreClients()) {
                std::cout << "Channel is full" << std::endl;
                throw CommandException(ERR_CHANNELISFULL(_client.getNickname(), chan->getName()));
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
                    throw CommandException(ERR_BADCHANNELKEY(_client.getNickname(), chan->getName()));
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

// void Command::handle_LIST() {}


Client * Command::getMatchingClient(std::string & username) const
{  
	for (std::set<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) 
    {
        if (username == (*it)->getNickname()) 
            return *it;
	}
	return NULL;
};

Channel * Command::getMatchingChannel(std::string & channelName, std::set<Channel *> & channels) const
{
	for (std::set<Channel *>::iterator it = channels.begin(); it != channels.end(); it++) 
    {
        if (channelName == (*it)->getName()) 
            return *it;
	}
	return NULL;    
}

void Command::handle_KICK() 
{

	// Parameters: <channel> *( "," <channel> ) <user> *( "," <user> ) [<comment>]

	//"you are banned" message, par ex quand on essaye de renvoyer un message
	//if there are several channels and that a few of them dont exist ? (try catch ?)

	int spacePos = _parameters.find(' ');
	int colonPos = _parameters.find(':');

	std::vector<std::string> paramChannelNames = Utilities::split(_parameters.substr(0, spacePos), ',');
	std::string paramUsername = _parameters.substr(spacePos + 1, colonPos - spacePos - 2); //removing the colon and the SPACE before it
	std::string reason = "";
	if (_parameters[colonPos + 1]) // if there is a comment
		reason = _parameters.substr(colonPos + 1);
	


	//Checking number and types of parameters 
	if (_parameters.empty())
		throw(CommandException(ERR_NEEDMOREPARAMS(_cmd)));
	if (paramChannelNames.empty() || paramUsername.empty())
		throw(CommandException(ERR_NOSUCHNICK(_parameters[0])));


	// Checking if ALL given channel names exist
	ChannelManager & cm = _client.getCM();
	std::set<Channel *> channels = cm.getChannels();

	for (std::vector<std::string>::iterator it = paramChannelNames.begin(); 
		it != paramChannelNames.end(); ++it)
	{
		std::set<Channel *>::iterator it2 = channels.begin();
		while (it2 != channels.end() && *it != (*it2)->getName())
			++it2;
		if (it2 == channels.end()) 
			throw(CommandException(ERR_NOSUCHNICK(*it)));
	}


	//Checking if the given username exist
	std::set<Client *>& allUsers = _client.getClients();
	std::set<Client *>::iterator it2 = allUsers.begin();

	while (it2 != allUsers.end() && paramUsername != (*it2)->getNickname())
		++it2;
	if (it2 == allUsers.end()) 
		throw(CommandException(ERR_NOSUCHNICK(paramUsername)));
	


	//Checking if client is operator and if the user to kick is in the channels
	Client * paramUser = getMatchingClient(paramUsername);

	for (size_t i = 0; i < paramChannelNames.size(); ++i)
	{
		Channel * paramChannel;
		paramChannel = getMatchingChannel(paramChannelNames[i], channels);

		if (!paramChannel->checkIfClientOperator(&_client))
			throw(CommandException(ERR_CHANOPRIVSNEEDED_kick(_client.getNickname(), paramChannel->getName())));
		if (!paramChannel->checkIfClientInChannel(paramUser))
			throw(CommandException(ERR_USERNOTINCHANNEL(paramUsername, paramChannel->getName())));

		paramChannel->banUser(paramUser);
		_client.send_message(RPL_KICK(_client.getPrefix(), paramChannel->getName(), paramUser->getNickname(),reason));
		(*paramUser).send_message(RPL_KICK(_client.getPrefix(), paramChannel->getName(), paramUser->getNickname(),reason));
	}

}

// void Command::handle_KILL() {}

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
    //   b: Not in subject, but adding so we can communicate with irssi.

    // Need to check if there are parameters. If not, throw an exception.
    if (_parameters.size() <= 1)
        throw CommandException(ERR_NEEDMOREPARAMS(_cmd));

    // basic parsing so get the channel or user we're modifying.
        // after split there should be at least 1 element.
        // First should be the channel or user, and the second should be the mode.
    std::vector<std::string> paramsVec = Utilities::split(_parameters, ' ');
    if (paramsVec.size() < 1)
        throw CommandException(ERR_NEEDMOREPARAMS(_cmd));
    if (paramsVec[0][0] != '#')
        // throw CommandException(ERR_NOSUCHCHANNEL(_client.getNickname(), paramsVec[0]));
        throw CommandException();

    // Check that the client is in the channel and is a channel operator for this channel
    Channel *chan = _client.getCM().getChannel(paramsVec[0]);
    if (chan == NULL)
        throw CommandException(ERR_NOSUCHCHANNEL(_client.getNickname(), paramsVec[0]));
    if (paramsVec.size() < 2)
        throw CommandException(RPL_CHANNELMODEIS(_client.getNickname(), chan->getName(), chan->getMode()));
    if (!chan->checkIfClientInChannel(&_client))
        throw CommandException(ERR_NOTONCHANNEL(paramsVec[0]));
    
    // Check if the mode is a ban mode request (we only repond to irssi during join)
    size_t pos = paramsVec[1].find('b');
    if (pos != std::string::npos && pos == 0 && paramsVec[1].size() == 1) {
        handle_MODE_b(chan);
        return ;
    }

    if (!chan->checkIfClientIsOp(&_client))
        throw CommandException(ERR_CHANOPRIVSNEEDED_gen(_client.getNickname(), paramsVec[0]));

    // Check the mode and perform the appropriate action
    // come in the form of +il-k for example. The positives first as a group, then the negatives
    bool positive = true;
    size_t argNum = 2;
    std::string channelModeIsStr = ":" + _client.getNickname() + "!~" + _client.getUsername() + "@" + _client.getHostname() + " MODE " + chan->getName() + " :";
    size_t strSize = channelModeIsStr.size();
    std::string modeArgs = "";
    for (size_t i = 0; i < paramsVec[1].size(); ++i) {
        switch (paramsVec[1][i]) {
            case '+':
                positive = true;
                channelModeIsStr += "+";
                break;
            case '-':
                positive = false;
                channelModeIsStr += "-";
                break;
            case 'i':
                if (handle_MODE_i(positive, chan)) {
                    channelModeIsStr += "i";
                }
                break;
            case 't':
                if (handle_MODE_t(positive, chan)) {
                    channelModeIsStr += "t";
                }
                break;
            case 'k':
                if (handle_MODE_k(positive, chan,
                    (positive && argNum < paramsVec.size() 
                    ? paramsVec[argNum++] : ""))) {
                    channelModeIsStr += "k";
                    if (positive)
                        modeArgs += (paramsVec[argNum - 1] + " ");
                }
                break;
            case 'o':
                if (handle_MODE_o(positive, chan,
                    (argNum < paramsVec.size() 
                    ? paramsVec[argNum++] : "") )) {
                    channelModeIsStr += "o";
                    modeArgs += (paramsVec[argNum - 1] + " ");
                }
                break;
            case 'l':
                if (handle_MODE_l(positive, chan,
                    (positive && argNum < paramsVec.size() 
                    ? paramsVec[argNum++] : "") )) {
                    channelModeIsStr += "l";
                    if (positive)
                        modeArgs += (paramsVec[argNum - 1] + " ");
                }
                break;
            case 'b':
                break ;
            default:
                // inform client that the mode is not recognized
                _client.send_message(ERR_UNKNOWNMODE(_client.getNickname(), paramsVec[1][i]));
                break;
        }
    }
    // check that the mode is not only + or - or that a - isnt added as the last character
    while (channelModeIsStr[channelModeIsStr.size() - 1] == '+' || channelModeIsStr[channelModeIsStr.size() - 1] == '-')
        channelModeIsStr.erase(channelModeIsStr.size() - 1);
    // make sure there are no '+-' or '-+' in the string. If so, remove.
    bool found = true;
    while (found) {
        found = false;
        std::string::size_type pos = channelModeIsStr.find("+-");

        if (pos != std::string::npos) {
            channelModeIsStr.erase(pos, 1);
            found = true;
        }
        pos = channelModeIsStr.find("-+");
        if (pos != std::string::npos) {
            channelModeIsStr.erase(pos, 1);
            found = true;
        }
    }
    // if the ending size is greater than the starting size, then we have a valid mode change
    if (channelModeIsStr.size() > strSize) {
        std::string message = channelModeIsStr + (modeArgs.size() > 1 ? (" " + modeArgs) : "") + "\r\n";
        _client.send_message(message);
        chan->forwardCommand(message, &_client);
    }
}

// void Command::handle_NAMES() {}


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
	// Parameters: <nickname>

	std::vector<std::string> params = Utilities::split(_parameters, ' ');
	std::string oldPrefix;

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
			// if it is the first connection (first NICK call)
			if (_client.getNickname() == "default")
				_client.setNickname(nickname + "_");
			else                                    // if the user tries to change his nickname (user already connected)
				throw(CommandException(ERR_NICKNAMEINUSE(nickname)));
		}
		else
		{
			oldPrefix = _client.getPrefix();
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
            funWelcomeMessage();
		}
	}

	_client.send_message(RPL_NICK(oldPrefix, _client.getNickname()));

}

// void Command::handle_NOTICE() {}

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
        throw CommandException(ERR_NOSUCHCHANNEL(_client.getNickname(), params[0]));
    }
    std::string message = _client.getPrefix() + " PART :" + chan->getName() + "\r\n";
    _client.send_message(message);
    _client.send_message(ERR_CHANOPRIVSNEEDED_part(_client.getNickname(), chan->getName()));
    chan->forwardCommand(message, &_client);
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
            funWelcomeMessage();
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
                throw CommandException(ERR_CANNOTSENDTOCHAN(_client.getNickname(), recipeints));
            }
        } else {
            // Channel does not exist
            std::cout << "Channel does not exist" << std::endl;
            throw CommandException(ERR_CANNOTSENDTOCHAN(_client.getNickname(), recipeints));
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

void Command::handle_TOPIC() 
{
	// Parameters: <channel> [ <topic> ]

	std::string topicCmd = "461 TOPIC TOPIC";

	if (_parameters.empty())
		throw(CommandException(ERR_NEEDMOREPARAMS(topicCmd)));

	int firstSpacePos = _parameters.find(' ');
	std::string channelName = _parameters.substr(0, firstSpacePos);

	int colonPos = _parameters.find(':');
	std::string topic = _parameters.substr(colonPos + 1);

	ChannelManager & cm = _client.getCM();
	std::set<Channel *> channels = cm.getChannels();
	Channel * channel = getMatchingChannel(channelName, channels);

	if (channel == NULL)
		throw(CommandException(ERR_NOSUCHCHANNEL(_client.getNickname(), channelName)));

	if (topic.empty()) //if there is no topic parameter
	{
		if ((channel->getTopic()).empty())
			_client.send_message(RPL_NOTOPIC(channel->getName()));
		else
		{
			Client * topicSetter = channel->getTopicSetter();
			_client.send_message(RPL_TOPIC(topicSetter->getPrefix(), channel->getName(), channel->getTopic()));
		}
	}
	else //if there is a topic parameter
	{
		if (!channel->checkIfClientInChannel(&_client))
			throw(CommandException(ERR_NOTONCHANNEL(channel->getName())));

		if (channel->getOnlyOperTopic()
            || (channel->onlyOperCanChangeTopic() && !channel->checkIfClientOperator(&_client)))
            // I dont know why but when we add the channel, irssi will kick the user. We just want to inform the user.
			throw(CommandException(ERR_CHANOPRIVSNEEDED_t(_client.getNickname(), channel->getName())));
		else
		{
			channel->setTopic(topic);
			channel->setTopicSetter(&_client);
			_client.send_message(RPL_TOPIC(_client.getPrefix(), channel->getName(), channel->getTopic()));
		}

	}
}

void Command::handle_USER()
{
	//Parameters: <user> <mode> <unused> <realname>

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

void Command::handle_VERSION() {
    // check params. if params does not match the server name (ft_irc), throw an error.
    if (_parameters.size() > 0 && _parameters != "ft_irc")
        throw CommandException(ERR_NOSUCHSERVER(_client.getNickname(), _parameters));
    std::string version = "1.0";
    std::string comments = "\n\nRoxane and Drew's amazing ft_irc server version 1.0 \n\
Ready to rock and roll!! \n\
Yowzaa!!! \n\
\n";
    _client.send_message(RPL_VERSION(version, "ft_irc", comments));
}

void Command::handle_WHO() {
    // input error checking (need parameters, and should be a channel, and user should be on the channel)
    if (_parameters.size() <= 1)
        throw CommandException(ERR_NEEDMOREPARAMS(_cmd));
    Channel *chan = _client.getCM().getChannel(_parameters);
    if (chan == NULL)
        throw CommandException(ERR_NOSUCHCHANNEL(_client.getNickname(), _parameters));
    if (!chan->checkIfClientInChannel(&_client))
        throw CommandException(ERR_NOTONCHANNEL(_parameters));

    for (std::map<Client*, bool>::iterator it = chan->getClients().begin(); it != chan->getClients().end(); ++it) {
        _client.send_message(RPL_WHOREPLY(  it->first->getNickname(), 
                                            chan->getName(), 
                                            it->first->getUsername(), 
                                            it->first->getHostname(), 
                                            "ft_irc", 
                                            ((it->second) ? "H@" : "H"), 
                                            "0", 
                                            it->first->getRealname()
                                        ));
    }
    _client.send_message(RPL_ENDOFWHO(_client.getNickname(), chan->getName()));
}

void Command::handle_WHOIS() {}
void Command::handle_WHOWAS() {}

// Mode flags

bool Command::handle_MODE_i(bool posFlag, Channel *chan) {
    std::cout << "Mode " << (posFlag ? "+" : "-") 
              << "i" << std::endl;
    if (!chan)
        // error check for channel address.
        return false;
    if (chan->getInviteOnly() != posFlag) {
        chan->setInviteOnly(posFlag);
        return true;
    }
    return false;
};

bool Command::handle_MODE_t(bool posFlag, Channel *chan) {
    std::cout << "Mode " << (posFlag ? "+" : "-") 
              << "t" << std::endl;
    if (!chan)
        // error check for channel address.
        return false;
    if (chan->getOnlyOperTopic() != posFlag) {
        chan->setOnlyOperTopic(posFlag);
        return true;
    }
    return false;
};

bool Command::handle_MODE_k(bool posFlag, Channel *chan, std::string arg) {
    std::cout << "Mode " << (posFlag ? "+" : "-") 
              << "k " << arg << std::endl;
    if (!chan->requiresKey() && posFlag && arg.size() > 0) {
        std::cout << "Setting key" << std::endl;
        chan->setKey(arg);
        return true;
    } else if (posFlag == false) {
        chan->setKey("");
        return true;
    }
    return false;
};

bool Command::handle_MODE_o(bool posFlag, Channel *chan, std::string arg) {
    // NOTE: Freenode only lets you enter one single user at a time, so we'll do the same
    std::cout << "Mode " << (posFlag ? "+" : "-") 
              << "o " << arg << std::endl;
    // make sure the argument is not empty
    if (arg.empty())
        return false;
    // Check if the argument is a valid user in the channel
    std::map<Client*, bool> &clients= chan->getClients();
    for (std::map<Client*, bool>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->first->getNickname() == arg && it->second != posFlag) {
            chan->setOperStatus(it->first, posFlag);
            return true;
        }
    }
    return false;
};

bool Command::handle_MODE_l(bool posFlag, Channel *chan, std::string arg) {
    std::cout << "Mode " << (posFlag ? "+" : "-") 
              << "l " << arg << std::endl;

    if (!posFlag && chan->getMaxClients() != -1) {
        chan->setMaxClients(-1);
        return true;
    } else if (!posFlag) {
        return false;
    }

    // Check if arg is a positive integer
    for (std::string::const_iterator it = arg.begin(); it != arg.end(); ++it) {
        if (!std::isdigit(*it))
            return false;
    }
    int newLimit = 0;
    std::istringstream iss(arg);
    iss >> newLimit;
    if (newLimit <= 0) {
        std::cout << "Invalid limit" << std::endl;
        return false;
    }

    if (posFlag) {
        // Assuming a method exists in Channel to check and set user limit
        if (chan->getMaxClients() != newLimit) {
            chan->setMaxClients(newLimit);
            return true;
        }
    }
    return false;
};

bool Command::handle_MODE_b(Channel *chan) {
    std::cout << "Mode b " << std::endl;
    _client.send_message(RPL_ENDOFBANLIST(_client.getNickname(), chan->getName()));
    return false;
};

void Command::funWelcomeMessage() const {

    std::stringstream ss;
    ss << "\
###########################################\n\
 \n\
Welcome to Roxane and Drew's ft_irc server!\n\
 \n\
_____________________________________ \n\
|    .___.___.    ._. .__   __      |\n\
|    [__   |       |  [__) /  `     |\n\
|    |     |      _|_ |  \\ \\__.     |\n\
|___________________________________|\n\
 \n\
";
#ifdef FUN_FLAGS
ss << "|" << BLUE << "OOOOOOOOOOOOOOO" << WHITE << "OOOOOOOOOOOOOOO" << RED << "OOOOOOOOOOOOOOO" << RESET << "| |" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << RED << "OOOOOOOOOOOOOOOOOOOOOOOOO" << RESET << "|\n\
|" << BLUE << "OOOOOOOOOOOOOOO" << WHITE << "OOOOOOOOOOOOOOO" << RED << "OOOOOOOOOOOOOOO" << RESET << "| |" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << BLUE << "@" << WHITE << "OOOOOOOOOOOOOOOOOOOOOOOOO" << RESET << "|\n\
|" << BLUE << "OOOOOOOOOOOOOOO" << WHITE << "OOOOOOOOOOOOOOO" << RED << "OOOOOOOOOOOOOOO" << RESET << "| |" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << RED << "OOOOOOOOOOOOOOOOOOOOOOOOO" << RESET << "|\n\
|" << BLUE << "OOOOOOOOOOOOOOO" << WHITE << "OOOOOOOOOOOOOOO" << RED << "OOOOOOOOOOOOOOO" << RESET << "| |" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << BLUE << "@" << WHITE << "OOOOOOOOOOOOOOOOOOOOOOOOO" << RESET << "|\n\
|" << BLUE << "OOOOOOOOOOOOOOO" << WHITE << "OOOOOOOOOOOOOOO" << RED << "OOOOOOOOOOOOOOO" << RESET << "| |" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << RED << "OOOOOOOOOOOOOOOOOOOOOOOOO" << RESET << "|\n\
|" << BLUE << "OOOOOOOOOOOOOOO" << WHITE << "OOOOOOOOOOOOOOO" << RED << "OOOOOOOOOOOOOOO" << RESET << "| |" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << BLUE << "@" << WHITE << "OOOOOOOOOOOOOOOOOOOOOOOOO" << RESET << "|\n\
|" << BLUE << "OOOOOOOOOOOOOOO" << WHITE << "OOOOOOOOOOOOOOO" << RED << "OOOOOOOOOOOOOOO" << RESET << "| |" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << WHITE << "*" << BLUE << "@" << RED << "OOOOOOOOOOOOOOOOOOOOOOOOO" << RESET << "|\n\
|" << BLUE << "OOOOOOOOOOOOOOO" << WHITE << "OOOOOOOOOOOOOOO" << RED << "OOOOOOOOOOOOOOO" << RESET << "| |" << WHITE << "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << RESET << "|\n\
|" << BLUE << "OOOOOOOOOOOOOOO" << WHITE << "OOOOOOOOOOOOOOO" << RED << "OOOOOOOOOOOOOOO" << RESET << "| |" << RED << "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << RESET << "|\n\
|" << BLUE << "OOOOOOOOOOOOOOO" << WHITE << "OOOOOOOOOOOOOOO" << RED << "OOOOOOOOOOOOOOO" << RESET << "| |" << WHITE << "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << RESET << "|\n\
|" << BLUE << "OOOOOOOOOOOOOOO" << WHITE << "OOOOOOOOOOOOOOO" << RED << "OOOOOOOOOOOOOOO" << RESET << "| |" << RED << "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << RESET << "|\n\
|" << BLUE << "OOOOOOOOOOOOOOO" << WHITE << "OOOOOOOOOOOOOOO" << RED << "OOOOOOOOOOOOOOO" << RESET << "| |" << WHITE << "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << RESET << "|\n\
|" << BLUE << "OOOOOOOOOOOOOOO" << WHITE << "OOOOOOOOOOOOOOO" << RED << "OOOOOOOOOOOOOOO" << RESET << "| |" << RED << "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << RESET << "|\n \n";
#endif
    ss << "This server is capable of handling the following commands:\n\
CAP, JOIN, KICK, MODE, NAMES, NICK, NOTICE, PART, PASS, PING, PRIVMSG, QUIT, TOPIC, USER, VERSION, WHO\n\
 \n\
The server is also capable of handling the following modes:\n\
i, t, k, o, l, b\n\
 \n\
Have fun chatting!\n\
 \n\
##########################################\n\
\n\
";

    std::string welcomeMessage = ss.str();

    std::vector<std::string> welcomeMessageVec = Utilities::split(welcomeMessage, '\n');
    for (std::vector<std::string>::iterator it = welcomeMessageVec.begin(); it != welcomeMessageVec.end(); ++it) {
        _client.send_message(*it + "\r\n");
    }
};


void Command::handle_LIST() {}
void Command::handle_KILL() {}
void Command::handle_NAMES() {}
void Command::handle_NOTICE() {}
void Command::handle_INFO() {}
