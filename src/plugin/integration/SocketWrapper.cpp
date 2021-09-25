#include "SocketWrapper.h"

namespace UKControllerPlugin::Integration {
    SocketWrapper::SocketWrapper(SOCKET socket) : socket(socket)
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

    auto SocketWrapper::Active() const -> bool
    {
        return this->active;
    }

    auto SocketWrapper::operator<<(std::string& message) -> SocketInterface&
    {
        auto lock = this->LockWriteSteam();
        this->writeStream << message;
        return *this;
    }

    auto SocketWrapper::operator>>(std::string& message) -> SocketInterface&
    {
        auto lock = this->LockReadStream();
        message = readStream.str();
        ResetStream(this->readStream);
        return *this;
    }

    auto SocketWrapper::operator>>(std::stringstream& inboundStream) -> SocketInterface&
    {
        auto lock = this->LockReadStream();
        inboundStream << this->readStream.str();
        ResetStream(this->readStream);
        return *this;
    }

    void SocketWrapper::ReadLoop()
    {
        int bytesReceived = 0;
        std::array<char, READ_BUFFER_SIZE> receiveBuffer{};
        do {
            bytesReceived = recv(this->socket, &receiveBuffer[0], READ_BUFFER_SIZE, 0);

            if (bytesReceived > 0) {
                auto lock = this->LockReadStream();
                if (this->readStream.eof()) {
                    this->readStream.clear();
                }

                this->readStream << std::string(receiveBuffer.cbegin(), receiveBuffer.cbegin() + bytesReceived);
            } else if (bytesReceived == 0) {
                LogInfo("Integration connection closing");
                this->active = false;
            } else {
                LogError("Failed to receive data from integration: " + std::to_string(WSAGetLastError()));
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

            int sendResult = send(this->socket, message.c_str(), static_cast<int>(message.size()), 0);
            if (sendResult == SOCKET_ERROR) {
                LogError("Failed to send on socket, socket will be shut down: " + std::to_string(WSAGetLastError()));
                this->active = false;
            }
        } while (this->active);
    }

    auto SocketWrapper::LockWriteSteam() -> std::lock_guard<std::mutex>
    {
        return std::lock_guard(this->writeStreamLock);
    }

    auto SocketWrapper::LockReadStream() -> std::lock_guard<std::mutex>
    {
        return std::lock_guard(this->readStreamLock);
    }

    auto SocketWrapper::GetMessageToWrite() -> std::string
    {
        std::string message;
        auto lock = this->LockWriteSteam();
        if (this->writeStream.tellp() == 0) {
            return message;
        }

        message = this->writeStream.str();
        ResetStream(this->writeStream);
        return message;
    }

    void SocketWrapper::ResetStream(std::stringstream& stream)
    {
        stream.str("");
        stream.clear();
    }
} // namespace UKControllerPlugin::Integration
