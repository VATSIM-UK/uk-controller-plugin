#pragma once
#include "websocket/WebsocketConnectionInterface.h"

namespace UKControllerPlugin {
    namespace Websocket {

        /*
            Class that handles all of UKCPs connections to the
            APIs websocket.
        */
        class WebsocketConnection : public UKControllerPlugin::Websocket::WebsocketConnectionInterface
        {
            public:
                WebsocketConnection(
                    std::string host,
                    std::string port
                );
                ~WebsocketConnection(void);

                bool IsConnected(void) const;

                // Inherited via WebsocketConnectionInterface
                void WriteMessage(std::string message) override;
                std::string GetNextMessage(void) override;
                std::chrono::seconds GetTimeSinceLastActivity(void) const override;
                void ForceDisconnect(void) override;

                // How often to attempt a reconnect if the server goes away
                const std::chrono::seconds reconnectAttemptInterval;

            private:

                void CloseHandler(boost::system::error_code ec);
                void ConnectHandler(boost::system::error_code ec);
                void HandshakeHandler(boost::system::error_code ec);
                void Loop(void);
                void MessageSentHandler(boost::system::error_code ec, std::size_t bytes_transferred);
                void ReadHandler(boost::beast::error_code ec, std::size_t bytes_transferred);
                void ResolveHandler(
                    boost::system::error_code ec,
                    boost::asio::ip::tcp::resolver::results_type results
                );

                void ProcessErrorCode(boost::system::error_code ec);

                // The websocket host
                const std::string host;

                // The websocket port
                const std::string port;

                // Incoming buffer
                boost::beast::multi_buffer incomingBuffer;

                // Io context
                boost::asio::io_context ioContext;

                // Resolving addresses
                boost::asio::ip::tcp::resolver tcpResolver;

                // The websocket itself
                boost::beast::websocket::stream<boost::asio::ip::tcp::socket> websocket;

                // The thread we're using to run the websocket.
                std::thread websocketThread;

                // Messages that are yet to be processed by the rest of the plugin
                std::queue<std::string> inboundMessages;

                // Protects the inbound messages queue
                std::mutex inboundMessageQueueGuard;

                // Messages that are to be sent
                std::queue<std::string> outboundMessages;

                // Protects the outbound messages queue
                std::mutex outboundMessageQueueGuard;

                // Are we connected
                bool connected = false;

                // Are we trying to connect at the moment?
                bool connectionInProgress;

                // Threads are running
                bool threadsRunning = true;

                // Is an async read in progress?
                bool asyncReadInProgress = false;

                // Is an async write in progress?
                bool asyncWriteInProgress = false;

                // The last time something happened
                std::chrono::system_clock::time_point lastActivityTime;

                // The next time to try reconnecting
                std::chrono::system_clock::time_point nextReconnectAttempt = 
                    (std::chrono::system_clock::time_point::min)();
        };
    }  // namespace Websocket
}  // namespace UKControllerPlugin
