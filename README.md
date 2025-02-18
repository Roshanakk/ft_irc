# ft_irc

ft_irc is a Internet Relay Chat (IRC) server implementation, a protocol for real-time messaging and communication in a distributed network environment.


## Features

- **Multi-client Support:** Manages concurrent client connections via sockets.

- **Message Broadcasting:** Delivers messages to all participants within a channel.

- **IRC Command Handling:** Implements commands like JOIN, KICK, PRIVMSG, NICK, QUIT, etc...

- **Channel Management:** Facilitates creation, joining, and leaving of communication channels.

- **Authentication:** Ensures secure client-server interaction through password-protected access.




## Run the project

```
git clone <repository_url>
cd ft_irc
make
./ircserver <port> <password>
```

## Usage


To connect to the IRC server, use any IRC client such as irssi.
Open the IRC client, then connect using the command:

```
/connect localhost <port> <password>
```
