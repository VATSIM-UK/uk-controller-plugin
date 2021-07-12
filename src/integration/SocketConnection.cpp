#include "pch/stdafx.h"
#include "integration/SocketConnection.h"

namespace UKControllerPlugin::Integration {
    SocketConnection::SocketConnection(SOCKET socket): socket(socket)
    {
        this->receiveThread = std::make_shared<std::thread>(&SocketConnection::ReceiveLoop, this);
    }

    SocketConnection::~SocketConnection()
    {
        if (this->active) {
            int shutdownResult = shutdown(this->socket, SD_BOTH);
            if (shutdownResult == SOCKET_ERROR) {
                LogError("Shutdown error shutting down socket: " + std::to_string(WSAGetLastError()));
            }
            this->active = false;
        }

        closesocket(this->socket);
        this->receiveThread->join();
    }

    bool SocketConnection::Active() const
    {
        return this->active;
    }

    std::string SocketConnection::Receive()
    {
        auto lock = this->LockStream();
        std::string messageToReturn;

        std::getline(this->dataStream, messageToReturn, '\0');
        if (this->dataStream.eof)
        std::string data;
        this->dataStream >> data;
        return data;
    }

    void SocketConnection::Send(std::string message)
    {
        // Check we're active
        if (!this->active) {
            return;
        }

        // Do the send and if we fail to, shut down
        int sendResult = send(this->socket, message.c_str(), message.size(), 0);
        if (sendResult == SOCKET_ERROR) {
            LogError("Failed to send on socket, socket will be shut down: " + std::to_string(WSAGetLastError()));
            this->active = false;
        }
    }

    std::lock_guard<std::mutex> SocketConnection::LockQueue()
    {
        return std::lock_guard(this->queueLock);
    }

    void SocketConnection::ReceiveLoop()
    {
        int bytesReceived;
        std::vector<char> receiveBuffer(4096);
        do {
            bytesReceived = recv(this->socket, &receiveBuffer[0], 4096, 0);

            if (bytesReceived > 0) {
                this->LockStream();
                if (std::find(receiveBuffer.begin(), receiveBuffer.end(), MESSAGE_DELIMITER) !=) 
                this->dataStream << receiveBuffer;
            } else if (bytesReceived == 0) {
                LogInfo("Integration connection closing");
                this->active = false;
            } else {
                LogError("Failed to receive data from integration: " + WSAGetLastError());
                this->active = false;
            }
        } while (this->active);
    }

    /*
     * Process data that we have received
     */
    void SocketConnection::ProcessReceivedData(std::vector<char>& data)
    {
        /*
         * Go through the data and find delimiters, split the data out into messages.
         */
        auto lock = this->LockQueue();
        std::vector<char>::iterator firstIt = data.begin();
        std::vector<char>::iterator it = data.begin();
        while ((it = std::find_if(it, data.end(), [this](const char& character) -> bool
        {
            return character == MESSAGE_DELIMITER;
        })) != data.end()) {
            if (this->incomingData.peek() != EOF) {
                std::string dataString;
                this->incomingData >> dataString;
                this->messages.push(dataString + std::string(firstIt, it));
            } else {
                this->messages.push(std::string(firstIt, it));
            }
            firstIt = it;
        }

        // Any leftover data, store it for now
        this->incomingData << std::string(firstIt, it);
    }
} // namespace UKControllerPlugin::Integration
