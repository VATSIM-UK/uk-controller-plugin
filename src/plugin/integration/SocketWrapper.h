#pragma once
#include "SocketInterface.h"

namespace UKControllerPlugin::Integration {

    /*
     * Implements the SocketInterface
     */
    class SocketWrapper : public SocketInterface
    {
        public:
        explicit SocketWrapper(SOCKET socket);
        ~SocketWrapper() override;
        bool Active() const override;
        SocketInterface& operator<<(std::string& message) override;
        SocketInterface& operator>>(std::string& message) override;
        SocketInterface& operator>>(std::stringstream& inboundStream) override;

        private:
        void ReadLoop();
        void WriteLoop();
        std::lock_guard<std::mutex> LockWriteSteam();
        std::lock_guard<std::mutex> LockReadStream();
        std::string GetMessageToWrite();

        // The socket
        SOCKET socket;

        // The input stream
        std::stringstream readStream;

        // The output stream
        std::stringstream writeStream;

        // Is the socket still active - should we keep it alive
        bool active = true;

        // Thread for reading from the socket
        std::shared_ptr<std::thread> readThread;

        // Thread for writing to the socket
        std::shared_ptr<std::thread> writeThread;

        // Mutex for the read stream
        std::mutex readStreamLock;

        // Mutex for the write stream
        std::mutex writeStreamLock;
    };
} // namespace UKControllerPlugin::Integration
