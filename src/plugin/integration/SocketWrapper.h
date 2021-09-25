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
        [[nodiscard]] auto Active() const -> bool override;
        auto operator<<(std::string& message) -> SocketInterface& override;
        auto operator>>(std::string& message) -> SocketInterface& override;
        auto operator>>(std::stringstream& inboundStream) -> SocketInterface& override;

        private:
        void ReadLoop();
        void WriteLoop();
        auto LockWriteSteam() -> std::lock_guard<std::mutex>;
        auto LockReadStream() -> std::lock_guard<std::mutex>;
        auto GetMessageToWrite() -> std::string;
        static void ResetStream(std::stringstream& stream);

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

        // How big a message we can read in one go
        static inline const int READ_BUFFER_SIZE = 4096;
    };
} // namespace UKControllerPlugin::Integration
