#include "Command.hpp"

/**********************************************************/
/*                CONSTRUCTORS & DESTRUCTOR               */
/**********************************************************/

Command::Command(Client & client, std::set<Client *>& clients)
	: _client(client), _clients(clients)
{

	std::string listCmds[22] = {"CAP", "INFO", "INVITE", "JOIN", "LIST", "KICK",
				"KILL", "MODE", "NAMES", "NICK", "NOTICE", 
				"PART", "PASS", "PING", "PRIVMSG", "TOPIC", "USER",
				"VERSION", "WHO", "WHOIS", "WHOWAS", "NB_CMDS" };

	void  (Command::*fctCmds[22])() = {&Command::handle_CAP, &Command::handle_INFO, &Command::handle_INVITE,
								&Command::handle_JOIN, &Command::handle_LIST, &Command::handle_KICK, &Command::handle_KILL, 
								&Command::handle_MODE, &Command::handle_NAMES, &Command::handle_NICK, &Command::handle_NOTICE, 
								&Command::handle_PART, &Command::handle_PASS, &Command::handle_PING, &Command::handle_PRIVMSG, 
								&Command::handle_TOPIC, &Command::handle_USER,
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
	// Need to check if there are parameters. If not, throw an exception.
	if (_parameters.size() <= 1)
		throw CommandException(ERR_NEEDMOREPARAMS(_cmd));

	// Split parameters by space
	std::vector<std::string> params = Utilities::split(_parameters, ' ');

	ChannelManager& cm = _client.getCM();
	Channel *chan = cm.getChannel(params[0]);

	// Check that user is not in too many channels
	if (cm.getClientChannelCount(&_client) >= cm.getMaxChannelsForClient()) {
		throw CommandException(ERR_TOOMANYCHANNELS(params[0]));
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
				// NOTE: no spaces are allowed in the password so we can just chck params[1]
			if (params.size() > 1 && chan->checkKey(params[1])) {
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
		cm.addChannel(params[0], &_client);
	}
	std::cout << "Number of channels: " << cm.getNumChannels() << std::endl;
}

void Command::handle_LIST() {}


Client * Command::getMatchingClient(std::string & username) const
{  
	for (std::set<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) 
    {
        if (username == (*it)->getNickname()) 
            return *it;
	}
	return NULL;
};

Channel * Command::getMatchingChannel(std::string & username, std::set<Channel *> & channels) const
{
	for (std::set<Channel *>::iterator it = channels.begin(); it != channels.end(); it++) 
    {
        if (username == (*it)->getName()) 
            return *it;
	}
	return NULL;    
}

void Command::handle_KICK() 
{
//    Parameters: <channel> *( "," <channel> ) <user> *( "," <user> )
//                [<comment>]


	//COMMENTAIRE apres le ':'
	//a voir si on associe le commentaire au parametre "banned",
	//pour recevoir la cause du ban si on essaye de renvoyer un message

	//"you are banned" message, par ex quand on essaye de renvoyer un message



	int spacePos = _parameters.find(' ');

	std::vector<std::string> paramChannelNames = Utilities::split(_parameters.substr(0, spacePos), ',');
	std::string paramUsername = _parameters.substr(spacePos + 1);
	paramUsername = paramUsername.substr(0, paramUsername.size() - 2); //removing the trailing ' :'

	for (size_t i = 0; i < paramChannelNames.size(); ++i)
	{
		std::cout << "paramChannelNames[i] = " << paramChannelNames[i] << '$' << std::endl;

	}
	std::cout << "user name = " << paramUsername << '$' << std::endl;


	//Checking number and types of parameters 
	if (_parameters.empty())
		throw(CommandException(ERR_NEEDMOREPARAMS(_cmd)));
	if (paramChannelNames.empty() || paramUsername.empty())
	{
		// std::cout << "C'est ICI L'ERREUR " << std::endl;
		throw(CommandException(ERR_NOSUCHNICK(_parameters[0])));
	}



	// Checking if ALL given channel names exist
	ChannelManager & cm = _client.getCM();
	std::set<Channel *> channels = cm.getChannels();

	for (std::vector<std::string>::iterator it = paramChannelNames.begin(); 
		it != paramChannelNames.end(); ++it)
	{
		
		std::set<Channel *>::iterator it2 = channels.begin();
		while (it2 != channels.end() && *it != (*it2)->getName())
			++it2;

		try
		{
			if (it2 == channels.end()) 
				throw(CommandException(ERR_NOSUCHNICK(*it)));
		}
		catch(const std::exception& e)
		{
			_client.send_message(e.what());
		}
		
	}


	//Checking if the given username exist
	std::set<Client *>& allUsers = _client.getClients();

	std::set<Client *>::iterator it2 = allUsers.begin();
	while (it2 != allUsers.end() && paramUsername != (*it2)->getNickname())
		++it2;

	try
	{
		if (it2 == allUsers.end()) 
			throw(CommandException(ERR_NOSUCHNICK(paramUsername)));
	}
	catch(const std::exception& e)
	{
		_client.send_message(e.what());
	}
	



	//Checking if client is operator and if the user to kick is in the channels

	Client * paramUser = getMatchingClient(paramUsername);


	for (size_t i = 0; i < paramChannelNames.size(); ++i)
	{
		Channel * paramChannel;
		paramChannel = getMatchingChannel(paramChannelNames[i], channels);

		try
		{
			if (!paramChannel->checkIfClientOperator(&_client))
				throw(CommandException(ERR_CHANOPRIVSNEEDED(paramChannel->getName())));

			if (!paramChannel->checkIfClientInChannel(paramUser))
				throw(CommandException(ERR_USERNOTINCHANNEL(paramUsername, paramChannel->getName())));
		}
		catch(const std::exception& e)
		{
			_client.send_message(e.what());
		}
		
		paramChannel->removeClient(paramUser);

	}


	
}


void Command::handle_KILL() {}


void Command::handle_MODE() {
	// Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>]
		//    o - give/take channel operator privileges;
		//    p - private channel flag;
		//    s - secret channel flag;
		//    i - invite-only channel flag;
		//    t - topic settable by channel operator only flag;
		//    n - no messages to channel from clients on the outside;
		//    m - moderated channel;
		//    l - set the user limit to channel;
		//    b - set a ban mask to keep users out;
		//    v - give/take the ability to speak on a moderated channel;
		//    k - set a channel key (password).
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
			// if it is the first connection (first NICK call)
			if (_client.getNickname() == "default")
				_client.setNickname(nickname + "_");
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


// NOTE We need to check that a user is in a channel before sending 
// a message to that channel.

		if (chan != NULL) {
			// Channel exists
			std::cout << "Channel exists" << std::endl;
			if (chan->checkIfClientInChannel(&_client)) {
				// Send message to all clients in channel
				chan->forwardMessage(message, &_client);
			} else {
				// User is not in channel
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