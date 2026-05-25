#include "NetworkManager.hpp"
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

NetworkManager::NetworkManager() {
    hostSocket = INVALID_SOCKET;
    isHost = false;
    isConnected = false;
}

NetworkManager::~NetworkManager() {
    if (hostSocket != INVALID_SOCKET) closesocket(hostSocket);
    for (SOCKET s : connectionSockets) closesocket(s);
    WSACleanup();
}

bool NetworkManager::Initialize() {
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

bool NetworkManager::StartHost(int port, int maxPlayers) {
    if (!Initialize()) return false;
    isHost = true;
    isConnected = true;
    hostSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    
    bind(hostSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(hostSocket, maxPlayers);
    return true;
}

void NetworkManager::AcceptClients(int expectedPlayers) {
    while (connectionSockets.size() < expectedPlayers) {
        sockaddr_in clientAddr;
        int clientSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(hostSocket, (struct sockaddr*)&clientAddr, &clientSize);
        if (clientSocket != INVALID_SOCKET) {
            connectionSockets.push_back(clientSocket);
        }
    }
}

bool NetworkManager::ConnectToHost(const std::string& ip, int port) {
    if (!Initialize()) return false;
    isHost = false;
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    serverAddr.sin_port = htons(port);
    
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        return false;
    }
    
    connectionSockets.push_back(clientSocket);
    isConnected = true;
    return true;
}

void NetworkManager::BroadcastMessage(const std::string& msg) {
    for (SOCKET s : connectionSockets) {
        send(s, msg.c_str(), msg.length(), 0);
    }
}

void NetworkManager::SendMessageTo(SOCKET sock, const std::string& msg) {
    send(sock, msg.c_str(), msg.length(), 0);
}

std::string NetworkManager::ReceiveMessage(SOCKET sock) {
    char buffer[4096] = {0};
    int bytesReceived = recv(sock, buffer, 4096, 0);
    if (bytesReceived > 0) {
        return std::string(buffer, bytesReceived);
    }
    return "";
}

bool NetworkManager::IsHost() const { return isHost; }
bool NetworkManager::IsConnected() const { return isConnected; }
std::vector<SOCKET>& NetworkManager::GetConnections() { return connectionSockets; }