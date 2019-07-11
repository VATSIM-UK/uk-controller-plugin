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
            std::string host,
            std::string port
        )
            : tcpResolver(ioContext), websocket(ioContext), host(host), port(port)
        {
            this->websocketThread = std::thread(std::bind(&WebsocketConnection::Loop, this));
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
            this->threadsRunning = false;
            this->websocketThread.join();
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
            LogInfo("Websocket handshake successful");
        }

        void WebsocketConnection::Loop(void)
        {
            while (this->threadsRunning) {

                this->ioContext.run();

                if (!this->connected || this->asyncReadInProgress) {
                    continue;
                }

                this->asyncReadInProgress = true;
                this->websocket.async_read(
                    this->incomingBuffer,
                    std::bind(
                        &WebsocketConnection::ReadHandler,
                        this,
                        std::placeholders::_1,
                        std::placeholders::_2
                    )
                );
            }
        }

        void WebsocketConnection::MessageSentHandler(boost::system::error_code ec, std::size_t bytes_transferred)
        {
            if (ec) {
                LogError("Failed to send message to websocket: " + ec.message());
                return;
            }

            this->connected = true;
            LogInfo("Message sent");
        }

        void WebsocketConnection::ReadHandler(boost::beast::error_code ec, std::size_t bytes_transferred)
        {
            this->asyncReadInProgress = false;

            if (ec) {
                LogError("Error reading from the websocket: " + ec.message());
                return;
            }

            if (bytes_transferred == 0) {
                return;
            }

            auto data = this->incomingBuffer.data();
            if (this->incomingBuffer.size() == 0) {
                return;
            }

            std::string testString = boost::beast::buffers_to_string(data);
            this->incomingBuffer.consume(bytes_transferred);
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

        void WebsocketConnection::TimedEventTrigger(void)
        {

        }
    }  // namespace Websocket
}  // namespace UKControllerPlugin
