#pragma once

#include <set>

#include "Channel.hpp"
#include "Client.hpp"

class Channel;
class Client;

class ChannelManager {
  public:
    ChannelManager(void);
    ~ChannelManager(void);

    void addChannel(std::string name, Client *client = NULL);
    void removeChannel(std::string name);
		void removeClientFromAllChannels(Client *client);

    // Getters //
    size_t getNumChannels(void);
    size_t getNumClientsInChannel(std::string name);
    Channel *getChannel(std::string name);
		int getClientChannelCount(Client *client);
    int getMaxChannelsForClient(void);

  private:
    std::set<Channel *> _channels;
    int _maxChannelsForClient;

};