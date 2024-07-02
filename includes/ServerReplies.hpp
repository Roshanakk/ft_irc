#define RPL_WELCOME(hostname, nick, prefix) (":" + hostname + " 001 " + nick + " :Welcome to the Internet Relay Network " + prefix + "\r\n")


#define ERR_UNKNOWNCOMMAND(hostname, cmd) (cmd + " :" + "Unknown command") 

// #define RPL_YOURHOST() "002 :Your host is <servername>, running version <ver>"
       
       
// #define RPL_CREATED() "003 This server was created <date>"

// #define RPL_MYINFO() "004 RPL_MYINFO <servername> <version> <available user modes> <available channel modes>"NB_CMDS


