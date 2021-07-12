#include "pch/stdafx.h"
#include "integration/IntegrationServer.h"

#include "IntegrationClientManager.h"
#include "integration/SocketConnection.h"
#include "integration/IntegrationClient.h"

namespace UKControllerPlugin::Integration {

    IntegrationServer::IntegrationServer(
        std::shared_ptr<IntegrationClientManager> manager
    ): manager(std::move(manager))
    {
        struct addrinfo *addressInfo = nullptr, *ptr = nullptr, hints;
        ZeroMemory(&hints, sizeof (hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        // Resolve the local address and port to be used by the server
        int getResult = getaddrinfo(nullptr, "52814", &hints, &addressInfo);
        if (getResult != 0) {
            LogError("getaddrinfo failed when started integration server: " + std::to_string(getResult));
            return;
        }

        // Create the socket
        this->serverSocket = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);
        if (this->serverSocket == INVALID_SOCKET) {
            LogError("Failed to initialise integration server socket: " + std::to_string(WSAGetLastError()));
            return;
        }

        // Bind it
        int bindResult = bind(this->serverSocket, addressInfo->ai_addr, static_cast<int>(addressInfo->ai_addrlen));

        // Free the struct that we don't need anymore
        freeaddrinfo(addressInfo);

        // Check the binding
        if (bindResult == SOCKET_ERROR) {
            LogError("Failed to bind integration server socket: " + std::to_string(WSAGetLastError()));
            closesocket(this->serverSocket);
            return;
        }

        // Listen on the socket
        if (listen(this->serverSocket, SOMAXCONN) == SOCKET_ERROR) {
            LogError("Failed to listen integration server socket: " + std::to_string(WSAGetLastError()));
            closesocket(this->serverSocket);
            return;
        }

        this->initialised = true;
        this->acceptThread = std::make_shared<std::thread>(&IntegrationServer::AcceptLoop, this);
    }

    IntegrationServer::~IntegrationServer()
    {
        if (this->initialised) {
            this->acceptingConnections = false;
            closesocket(this->serverSocket);
            LogInfo("Closed integration server socket");
        }
    }

    void IntegrationServer::AcceptLoop() const
    {
        while (this->acceptingConnections) {
            SOCKET integrationSocket = accept(integrationSocket, nullptr, nullptr);
            if (integrationSocket == INVALID_SOCKET) {
                LogError("Failed to accept integration server connection: " + std::to_string(WSAGetLastError()));
                closesocket(integrationSocket);
                continue;
            }

            this->manager->AddClient(
                std::make_shared<IntegrationClient>(
                    std::make_shared<TcpSocketConnection>(integrationSocket)
                )
            );
        }
    }
} // namespace UKControllerPlugin::Integration
