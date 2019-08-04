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
            Called when we try to connect to the websocket
        */
        void WebsocketConnection::ConnectHandler(boost::system::error_code ec)
        {
            if (ec) {
                LogWarning("Websocket: connection error");
                this->ProcessErrorCode(ec);
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

        /*
            Called after connection to the websocket to negotiate a protocol upgrade
        */
        void WebsocketConnection::HandshakeHandler(boost::system::error_code ec)
        {
            if (ec) {
                LogWarning("Websocket: handshake error");
                this->ProcessErrorCode(ec);
                return;
            }

            this->connected = true;
            LogInfo("Websocket handshake successful");
        }

        /*
            Loop that runs to send any outgoing messages and read
            any incoming message
        */
        void WebsocketConnection::Loop(void)
        {
            while (this->threadsRunning) {

                this->ioContext.run();

                if (!this->connected) {
                    continue;
                }

                // Process an inbound message if we can
                if (!this->asyncReadInProgress) {
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

                // Process an outbound message if we can
                if (!this->asyncWriteInProgress && !this->outboundMessages.empty()) {
                    std::lock_guard<std::mutex> lock(this->outboundMessageQueueGuard);
                    this->asyncWriteInProgress = true;
                    this->websocket.async_write(
                        boost::asio::buffer(this->outboundMessages.front()),
                        std::bind(
                            &WebsocketConnection::MessageSentHandler,
                            this,
                            std::placeholders::_1,
                            std::placeholders::_2
                        )
                    );
                    LogDebug("Sending websocket message: " + this->outboundMessages.front());
                    this->outboundMessages.pop();
                }
            }
        }

        /*
            Called when a message has been sent by the websocket.
        */
        void WebsocketConnection::MessageSentHandler(boost::system::error_code ec, std::size_t bytes_transferred)
        {
            this->asyncWriteInProgress = true;
            if (ec) {
                LogWarning("Websocket: message sending error");
                this->ProcessErrorCode(ec);
                return;
            }
        }

        /*
            Called when a message is received. Add it to the inbound message queue.
        */
        void WebsocketConnection::ReadHandler(boost::beast::error_code ec, std::size_t bytes_transferred)
        {
            if (ec) {
                this->ProcessErrorCode(ec);
                this->asyncReadInProgress = false;
                return;
            }

            if (bytes_transferred == 0) {
                this->asyncReadInProgress = false;
                return;
            }

            LogDebug("Incoming websocket message: " + boost::beast::buffers_to_string(this->incomingBuffer.data()));
            std::lock_guard<std::mutex> lock(this->inboundMessageQueueGuard);
            this->inboundMessages.push(boost::beast::buffers_to_string(this->incomingBuffer.data()));
            this->incomingBuffer.consume(bytes_transferred);
            this->asyncReadInProgress = false;
        }

        /*
            Handle when we resolve the websocket host
        */
        void WebsocketConnection::ResolveHandler(
            boost::system::error_code ec,
            boost::asio::ip::tcp::resolver::results_type results
        ) {
            if (ec) {
                LogWarning("Websocket: resolve error");
                this->ProcessErrorCode(ec);
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

        /*
            Add a message to the outbound queue
        */
        void WebsocketConnection::WriteMessage(std::string message)
        {
            std::lock_guard<std::mutex> lock(this->inboundMessageQueueGuard);
            this->outboundMessages.push(message);
        }

        /*
            Returns the next message on the queue
        */
        std::string WebsocketConnection::GetNextMessage(void)
        {
            std::lock_guard<std::mutex> lock(this->inboundMessageQueueGuard);
            if (this->inboundMessages.empty()) {
                return this->noMessage;
            }
            std::string message = this->inboundMessages.front();
            this->inboundMessages.pop();
            return message;
        }

        /*
            Processes an error code from the websocket.
        */
        void WebsocketConnection::ProcessErrorCode(boost::system::error_code ec)
        {
            // Handle disconnection
            if (ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset) {
                this->connected = false;
                LogWarning("Disconnected from websocket: " + ec.message());
                return;
            }

            LogWarning("Error code from websocket: " + std::to_string(ec.value()) + " (" + ec.message() + ")");
        }
    }  // namespace Websocket
}  // namespace UKControllerPlugin
