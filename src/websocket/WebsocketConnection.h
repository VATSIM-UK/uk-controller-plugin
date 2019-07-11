#pragma once
#include "websocket/WebsocketConnectionInterface.h"
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin {
    namespace Websocket {

        /*
            Class that handles all of UKCPs connections to the
            APIs websocket.
        */
        class WebsocketConnection : public UKControllerPlugin::Websocket::WebsocketConnectionInterface,
            public UKControllerPlugin::TimedEvent::AbstractTimedEvent
        {
            public:
                WebsocketConnection(
                    std::string host,
                    std::string port
                );
                ~WebsocketConnection(void);

                void ConnectHandler(boost::system::error_code ec);
                void HandshakeHandler(boost::system::error_code ec);
                void Loop(void);
                void MessageSentHandler(boost::system::error_code ec, std::size_t bytes_transferred);
                void ReadHandler(boost::beast::error_code ec, std::size_t bytes_transferred);
                void ResolveHandler(
                    boost::system::error_code ec,
                    boost::asio::ip::tcp::resolver::results_type results
                );
                bool IsConnected(void) const;

                // Inherited via WebsocketConnectionInterface
                bool WriteMessage(std::string message) override;
                UKControllerPlugin::Websocket::WebsocketMessage GetNextMessage(void) override;

            private:

                void ProcessErrorCode(boost::system::error_code ec);

                std::string message;

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

                // Are we connected
                bool connected = false;

                // Threads are running
                bool threadsRunning = true;

                // Is an async read in progress?
                bool asyncReadInProgress = false;

                // Inherited via AbstractTimedEvent
                virtual void TimedEventTrigger(void) override;
        };
    }  // namespace Websocket
}  // namespace UKControllerPlugin
