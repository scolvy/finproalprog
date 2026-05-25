#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

#include <winsock2.h>
#include <string>
#include <vector>

class NetworkManager {
private:
    SOCKET hostSocket;
    std::vector<SOCKET> connectionSockets;
    bool isHost;
    bool isConnected;

public:
    NetworkManager();
    ~NetworkManager();
    bool Initialize();
    bool StartHost(int port, int maxPlayers);
    bool ConnectToHost(const std::string& ip, int port);
    void AcceptClients(int expectedPlayers);
    void BroadcastMessage(const std::string& msg);
    void SendMessageTo(SOCKET sock, const std::string& msg);
    std::string ReceiveMessage(SOCKET sock);
    bool IsHost() const;
    bool IsConnected() const;
    std::vector<SOCKET>& GetConnections();
};

#endif