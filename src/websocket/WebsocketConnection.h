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
                    boost::asio::io_context & ioContext,
                    std::string host,
                    std::string port
                );
                ~WebsocketConnection(void);

                void ConnectHandler(boost::system::error_code ec);
                void HandshakeHandler(boost::system::error_code ec);
                void MessageSentHandler(boost::system::error_code ec, std::size_t bytes_transferred);
                void ResolveHandler(
                    boost::system::error_code ec,
                    boost::asio::ip::tcp::resolver::results_type results
                );
                bool IsConnected(void) const;

                // Inherited via WebsocketConnectionInterface
                bool WriteMessage(std::string message) override;
                UKControllerPlugin::Websocket::WebsocketMessage GetNextMessage(void) override;

            private:

                std::string message;

                // The websocket host
                const std::string host;

                // The websocket port
                const std::string port;

                // Resolving addresses
                boost::asio::ip::tcp::resolver tcpResolver;

                // The websocket itself
                boost::beast::websocket::stream<boost::asio::ip::tcp::socket> websocket;

                // Are we connected
                bool connected = false;
        };
    }  // namespace Websocket
}  // namespace UKControllerPlugin
