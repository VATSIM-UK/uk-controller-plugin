#include "pch/pch.h"
#include "integration/SocketWrapper.h"

namespace UKControllerPlugin::Integration {
    SocketWrapper::SocketWrapper(SOCKET socket): socket(socket)
    {
        this->writeThread = std::make_shared<std::thread>(&SocketWrapper::WriteLoop, this);
        this->readThread = std::make_shared<std::thread>(&SocketWrapper::ReadLoop, this);
    }

    SocketWrapper::~SocketWrapper()
    {
        if (this->active) {
            LogDebug("Shutting down socket");
            int shutdownResult = shutdown(this->socket, SD_BOTH);
            if (shutdownResult == SOCKET_ERROR) {
                LogError("Shutdown error shutting down socket: " + std::to_string(WSAGetLastError()));
            }
            this->active = false;
        }

        LogDebug("Closing socket");
        closesocket(this->socket);
        this->readThread->join();
        this->writeThread->join();
    }

    bool SocketWrapper::Active() const
    {
        return this->active;
    }

    SocketInterface& SocketWrapper::operator<<(std::string& message)
    {
        auto lock = this->LockWriteSteam();
        if (this->writeStream.eof()) {
            this->writeStream.clear();
        }

        this->writeStream << message;
        return *this;
    }

    SocketInterface& SocketWrapper::operator>>(std::string& message)
    {
        auto lock = this->LockReadStream();
        this->readStream >> message;
        return *this;
    }

    SocketInterface& SocketWrapper::operator>>(std::stringstream& inboundStream)
    {
        auto lock = this->LockReadStream();

        std::string dataString;
        this->readStream >> dataString;
        inboundStream << dataString;
        return *this;
    }

    void SocketWrapper::ReadLoop()
    {
        int bytesReceived;
        std::array<char, 4096> receiveBuffer;
        do {
            bytesReceived = recv(this->socket, &receiveBuffer[0], 4096, 0);

            if (bytesReceived > 0) {
                auto lock = this->LockReadStream();
                if (this->readStream.eof()) {
                    this->readStream.clear();
                }

                this->readStream << std::string(&receiveBuffer[0], &receiveBuffer[bytesReceived]);
            } else if (bytesReceived == 0) {
                LogInfo("Integration connection closing");
                this->active = false;
            } else {
                LogError("Failed to receive data from integration: " + WSAGetLastError());
                this->active = false;
            }
        } while (this->active);
    }

    void SocketWrapper::WriteLoop()
    {
        std::string message;
        do {
            // Sleep if nothing to do to give us a break.
            if ((message = this->GetMessageToWrite()).empty()) {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                continue;
            }

            int sendResult = send(this->socket, message.c_str(), message.size(), 0);
            if (sendResult == SOCKET_ERROR) {
                LogError("Failed to send on socket, socket will be shut down: " + std::to_string(WSAGetLastError()));
                this->active = false;
            }
        } while (this->active);
    }

    std::lock_guard<std::mutex> SocketWrapper::LockWriteSteam()
    {
        return std::lock_guard(this->writeStreamLock);
    }

    std::lock_guard<std::mutex> SocketWrapper::LockReadStream()
    {
        return std::lock_guard(this->readStreamLock);
    }

    std::string SocketWrapper::GetMessageToWrite()
    {
        std::string message;
        auto lock = this->LockWriteSteam();
        if (this->writeStream.eof()) {
            return message;
        }

        this->writeStream >> message;
        return message;
    }
} // namespace UKControllerPlugin::Integration
