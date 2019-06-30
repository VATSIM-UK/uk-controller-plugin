#pragma once
#include "pch/stdafx.h"
#include "websocket/WebsocketConnection.h"

namespace UKControllerPlugin {
    namespace Websocket {

        boost::beast::flat_buffer buffer_;

        /*
            Start connection to the websocket by resolving the host
        */
        WebsocketConnection::WebsocketConnection(
            boost::asio::io_context & ioContext,
            std::string host,
            std::string port
        )
            : tcpResolver(ioContext), websocket(ioContext), host(host), port(port)
        {
            this->tcpResolver.async_resolve(
                host,
                port,
                std::bind(
                    &WebsocketConnection::ResolveHandler,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2
                )
            );
        }

        /*
            Kill the websocket connection, do this synchronously
        */
        WebsocketConnection::~WebsocketConnection(void)
        {
            this->websocket.close(boost::beast::websocket::close_code::normal);
            LogInfo("Disconnected from websocket");
        }

        /*
            Handle connections to the websocket.
        */
        void WebsocketConnection::ConnectHandler(boost::system::error_code ec)
        {
            if (ec) {
                LogError("Unable to connect to the websocket");
                return;
            }
            
            // Perform the websocket handshake
            this->websocket.async_handshake(
                this->host,
                "/app/ukcpwebsocket",
                std::bind(
                    &WebsocketConnection::HandshakeHandler,
                    this,
                    std::placeholders::_1
                )
            );
            LogInfo("Successfully connected to the websocket");
        }

        void WebsocketConnection::HandshakeHandler(boost::system::error_code ec)
        {
            if (ec) {
                LogError("Unable to handshake on the websocket");
                std::string test = ec.message();
                return;
            }

            nlohmann::json data;
            data["event"] = "pusher:subscribe";
            data["data"]["channel"] = "test-channel";
            this->message = data.dump();
            this->websocket.async_write(
                boost::asio::buffer(this->message),
                std::bind(
                    &WebsocketConnection::MessageSentHandler,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2
                )
            );
            LogInfo("Handshake successful");
        }

        void WebsocketConnection::MessageSentHandler(boost::system::error_code ec, std::size_t bytes_transferred)
        {
            if (ec) {
                LogError("Failed to send message to websocket");
                std::string test = ec.message();
                return;
            }

            this->connected = true;
            LogInfo("Message sent");
        }

        /*
            Handle when we resolve the websocket host
        */
        void WebsocketConnection::ResolveHandler(
            boost::system::error_code ec,
            boost::asio::ip::tcp::resolver::results_type results
        ) {
            if (ec) {
                LogError("Unable to resolve websocket");
                std::string err = ec.message();
                return;
            }

            boost::asio::async_connect(
                this->websocket.next_layer(),
                results.begin(),
                results.end(),
                std::bind(
                    &WebsocketConnection::ConnectHandler,
                    this,
                    std::placeholders::_1
                )
            );
        }

        bool WebsocketConnection::IsConnected(void) const
        {
            return this->connected;
        }

        bool WebsocketConnection::WriteMessage(std::string message)
        {
            return false;
        }

        UKControllerPlugin::Websocket::WebsocketMessage WebsocketConnection::GetNextMessage(void)
        {
            return UKControllerPlugin::Websocket::WebsocketMessage();
        }
    }  // namespace Websocket
}  // namespace UKControllerPlugin
