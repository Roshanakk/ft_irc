/************************************************/
// Error Replies (Section 6.1 of RFC 1459)
/************************************************/
#define ERR_NOSUCHNICK(nick) (nick + " :No such nick/channel\r\n")
#define ERR_NOSUCHSERVER(nick, servername) (":ft_irc 402 " + nick + " " + servername + " :No such server\r\n")
#define ERR_NOSUCHCHANNEL(nick, channel) (":ft_irc 403 " + nick + " " + channel + " :No such channel\r\n")
#define ERR_CANNOTSENDTOCHAN(nick, channel) (":ft_irc 404 " + nick + " " + channel + " :Cannot send to channel\r\n")
#define ERR_TOOMANYCHANNELS(channel) (channel + " :You have joined too many channels\r\n")
#define ERR_WASNOSUCHNICK(nick) (nick + " :There was no such nickname\r\n")
#define ERR_TOOMANYTARGETS(target) (target + " :Duplicate recipients. No message delivered\r\n")
#define ERR_NOORIGIN() (":No origin specified\r\n")
#define ERR_NORECIPIENT(command) (":No recipient given (" + command + ")\r\n")
#define ERR_NOTEXTTOSEND() (":No text to send\r\n")
#define ERR_NOTOPLEVEL(domain) (domain + " :No toplevel domain specified\r\n")
#define ERR_WILDTOPLEVEL(domain) (domain + " :Wildcard in toplevel domain\r\n")
#define ERR_UNKNOWNCOMMAND(hostname, cmd) (cmd + " :" + "Unknown command\r\n") 
#define ERR_NOMOTD() (":MOTD File is missing\r\n")
#define ERR_NOADMININFO(servername) (servername + " :No administrative info available\r\n")
#define ERR_FILEERROR(fileop, file) (":File error doing " + fileop + " on " + file + "\r\n")
#define ERR_NONICKNAMEGIVEN() (":No nickname given\r\n")
#define ERR_ERRONEUSNICKNAME(nick) (nick + " :Erroneus nickname\r\n")
#define ERR_NICKNAMEINUSE(nick) (":ft_irc 433 * " + nick + " :Nickname is already in use.\r\n")

#define ERR_NICKCOLLISION(nick) (nick + " :Nickname collision KILL\r\n")

#define ERR_USERNOTINCHANNEL(nick, channel) (nick + " " + channel + " :They aren't on that channel\r\n")
#define ERR_NOTONCHANNEL(channel) (channel + " :You're not on that channel\r\n")
#define ERR_USERONCHANNEL(nick, channel) (nick + " " + channel + " :is already on channel\r\n")
#define ERR_NOLOGIN(nick) (nick + " :User not logged in\r\n")
#define ERR_SUMMONDISABLED() (":SUMMON has been disabled\r\n")
#define ERR_USERSDISABLED() (":USERS has been disabled\r\n")
#define ERR_NOTREGISTERED() (":You have not registered\r\n")
#define ERR_NEEDMOREPARAMS(nick, command) (":ft_irc 461 " + nick + " " + command + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED() (":You may not reregister\r\n")
#define ERR_NOPERMFORHOST() (":Your host isn't among the privileged\r\n")
#define ERR_PASSWDMISMATCH() (":Password incorrect\r\n")
#define ERR_PASSWDNEEDED() ("Password needed\r\n")
#define ERR_YOUREBANNEDCREEP() (":You are banned from this server\r\n")
#define ERR_KEYSET(channel) (channel + " :Channel key already set\r\n")
#define ERR_CHANNELISFULL(nick, channel) (":ft_irc 471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_UNKNOWNMODE(nick, mode) (":ft_irc 472 " + nick + " " + mode + " :is not a recognised channel mode.\r\n")
#define ERR_INVITEONLYCHAN(nick, channel) (":ft_irc 473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n")
#define ERR_BANNEDFROMCHAN(channel) (channel + " :Cannot join channel (+b)\r\n")
#define ERR_BADCHANNELKEY(user, channel) (":ft_irc 475 " + user + " " + channel + " :Cannot join channel (incorrect channel key)\r\n")
#define ERR_NOPRIVILEGES() (":Permission Denied- You're not an IRC operator\r\n")
// freenode uses multiple 482 responses, so we will use the same format
// Also note that if we include the nick in the response before the channel, we will kick the user from the channel.
#define ERR_CHANOPRIVSNEEDED_gen(nick, channel) (":ft_irc 482 " + channel + " :You must have channel operator status to set or unset channel mode\r\n")
#define ERR_CHANOPRIVSNEEDED_i(nick, channel) (":ft_irc 482 " + channel + " :You must have channel operator status to set or unset channel mode i\r\n")
#define ERR_CHANOPRIVSNEEDED_t(nick, channel) (":ft_irc 482 " + channel + " :You do not have access to change the topic on this channel\r\n")
#define ERR_CHANOPRIVSNEEDED_kick(nick, channel) (":ft_irc 482 " + channel + " :You do not have the right to kick users\r\n")
// fix for part not making irssi leave the channel
#define ERR_CHANOPRIVSNEEDED_part(nick, channel) (":ft_irc 482 " + nick + " " + channel + " :part\r\n")

#define ERR_CHANOPRIVSNEEDED(nick, channel) (":ft_irc 482 " + nick + " " + channel + " :You're not channel operator\r\n")

#define ERR_CANTKILLSERVER() (":You cant kill a server!\r\n")
#define ERR_NOOPERHOST() (":No O-lines for your host\r\n")
#define ERR_UMODEUNKNOWNFLAG() (":Unknown MODE flag\r\n")
#define ERR_USERSDONTMATCH() (":Cant change mode for other users\r\n")
#define ERR_RESTRICTED() (":Your connection is restricted!")

/****************************************************************/
// Command Responses (Section 6.2 of RFC 1459)
// Responses can be variable, not defining right now. 
// This may not be as straightforward as the error replies.
/****************************************************************/

// Connection Confirmation (Not in RFC 1459)
#define RPL_WELCOME(hostname, nick, prefix) (":" + hostname + " 001 " + nick + " :Welcome to the Internet Relay Network " + prefix + "\r\n")
// #define RPL_YOURHOST() "002 :Your host is <servername>, running version <ver>"
// #define RPL_CREATED() "003 This server was created <date>"
// #define RPL_MYINFO() "004 RPL_MYINFO <servername> <version> <available user modes> <available channel modes>"NB_CMDS
#define RPL_TOPIC(prefix, channel, topic) (prefix + " TOPIC " + channel + " :" + topic + "\r\n")
#define RPL_NOTOPIC(channel) (":ft_irc 331 " + channel + " :No topic is set\r\n")
#define RPL_NAMREPLY(nick, channel, listNicks) (":ft_irc 353 " + nick + " = " + channel + " :" + listNicks + "\r\n")
#define RPL_NICK(prefix, nickname) (prefix + " NICK " + nickname + "\r\n")
#define RPL_KICK(user_prefix, channel, kicked, reason) (user_prefix + " KICK " + channel + " " + kicked + " " + reason + "\r\n")
#define RPL_WHOWASUSER(nickname, whowasNick, whowasHost, realname) ("ft_irc: 314 " + nickname + " " + whowasNick + " " + whowasHost + " :" + realname + "\r\n")

#define RPL_WHOIS(nickname, takenNick, username, hostname, realname) (":ft_irc 311 " + nickname + " " + takenNick + " " + username + " " + hostname + " * :" + realname + "\r\n")

#define RPL_USERHOST(nickname, takenNick) (":ft_irc 312 " + nickname + " " + takenNick + " ft_irc :ft_irc\r\n")
#define RPL_ENDOFWHOIS(nickname, takenNick) (":ft_irc 318 " + nickname + " " + takenNick + " :End of /WHOIS list.\r\n")
#define RPL_TOPICUSAGE(nick) (":ft_irc 650 " + nick + " TOPIC :<channel> [:<topic>]\r\n")

// #define RPL_INVITE(userPrefix, userToInvite, channel) (userPrefix + " INVITE " + userToInvite + " " + channel + "\r\n")
// #define RPL_INVITING(hostname, user, channel, userToInvite) (":" + hostname + " 341 " + user + " " + userToInvite + " " + channel + "\r\n")


// #define RPL_INVITE(userPrefix, userToInvite, channel) (userPrefix + " INVITE " + userToInvite + " :" + channel + "\r\n")
// #define RPL_INVITING(hostname, user, channel, userToInvite) (":" + hostname + " 341 " + user + " " + userToInvite + " :" + channel + "\r\n")

//inviter : nini
//invited : jojo

// >> :*.freenode.net 341 nini jojo :#disney
// :hostname 341 inviter invited :channel

// >> :nini!~rraffi-k@freenode-3dg.s40.6vib9m.IP INVITE jojo :#disney
// prefox INVITE invited :channel

// #define RPL_INVITING(userPrefix, user, userToInvite, channel) (userPrefix + " 341 " + user + " " + userToInvite + " " + channel + "\r\n")
// #define RPL_INVITE(channel, userToInvite) (":ft_irc 341 " + userToInvite + "\r\n")

#define RPL_INVITING(user, userToInvite, channel) (":ft_irc 341 " + user + " " + userToInvite + " " + channel + "\r\n")
#define RPL_INVITE(userPrefix, userToInvite, channel) (userPrefix + " INVITE " + userToInvite + " " + channel + "\r\n")




#define RPL_ENDOFWHOWAS(nickname) (nickname + " :End of WHOWAS\r\n")
#define RPL_VERSION(version, hostname, comments) (":ft_irc 351 " + version + " " + hostname + " :" + comments + "\r\n")
#define RPL_ENDOFNAMES(nick, channel) (":ft_irc 366 " + nick + " " + channel + " :End of /NAMES list.\r\n")
// >> :*.freenode.net 324 yo1 #heythereguys :+nt$
#define RPL_CHANNELMODEIS(nick, channel, mode) (":ft_irc 324 " + nick + " " + channel + " :" + mode + "\r\n")
#define RPL_WHOREPLY(nick, channel, user, host, server, mode, hopCount, realName) (":ft_irc 352 " + nick + " " + channel + " " + user + " " + host + " " + server + " " + nick + " " + mode + " :" + hopCount + " " + realName + "\r\n")
#define RPL_ENDOFWHO(nick, channel) (":ft_irc 315 " + nick + " " + channel + " :End of /WHO list.\r\n")
#define RPL_ENDOFBANLIST(nick, channel) (":ft_irc 368 " + nick + " " + channel + " :End of channel ban list\r\n")

#define ERR_