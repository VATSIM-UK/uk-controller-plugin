#pragma once
#include "pch/stdafx.h"
#include "websocket/WebsocketConnection.h"
#include "update/PluginVersion.h"

using UKControllerPlugin::Plugin::PluginVersion;

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
            : host(host), port(port), reconnectAttemptInterval(30), idleTimeout(30),
            sslContext(boost::asio::ssl::context::tlsv12_client),
            workGuard(boost::asio::make_work_guard(this->ioContext))
        {
            this->ResetWebsocket();
            this->processThread = std::thread(std::bind(&WebsocketConnection::Loop, this));
            this->websocketThread = std::thread(std::bind(&WebsocketConnection::RunWebsocket, this));
        }

        /*
            Kill the websocket connection, do this synchronously
        */
        WebsocketConnection::~WebsocketConnection(void)
        {
            this->threadsRunning = false;
            // Stop the IO Context and wait for threads to join
            this->ioContext.stop();
            this->processThread.join();
            this->websocketThread.join();

            LogInfo("Disconnected from websocket");
        }

        /*
            Handle the connection closing
        */
        void WebsocketConnection::CloseHandler(boost::system::error_code ec)
        {
            if (ec) {
                this->ProcessErrorCode(ec);
            }

            this->ResetWebsocket();
            LogInfo("Force closed websocket connection");
        }

        /*
            Called when we try to connect to the websocket
        */
        void WebsocketConnection::ConnectHandler(boost::system::error_code ec)
        {
            if (ec) {
                LogWarning("Websocket: connection error");
                this->connectionInProgress = false;
                this->ProcessErrorCode(ec);
                return;
            }

            // Perform the SSL handshake with a fair timeout
            boost::beast::get_lowest_layer(*this->websocket).expires_after(std::chrono::seconds(30));
            this->websocket->next_layer().async_handshake(
                boost::asio::ssl::stream_base::client,
                std::bind(
                    &WebsocketConnection::SSLHandshakeHandler,
                    this,
                    std::placeholders::_1
                )
            );
            LogInfo("Successfully connected to the websocket");
        }

        /*
            Reset the websocket, ready for a reconnection attempt
        */
        void WebsocketConnection::ResetWebsocket(void)
        {
            this->connected = false;
            this->connectionInProgress = false;
            this->asyncReadInProgress = false;
            this->asyncWriteInProgress = false;

            this->tcpResolver.reset(new boost::asio::ip::tcp::resolver(boost::asio::make_strand(ioContext)));
            this->websocket.reset(
                new boost::beast::websocket::stream<boost::beast::ssl_stream<boost::beast::tcp_stream>>(
                    boost::asio::make_strand(ioContext), this->sslContext
                )
            );

            this->SetIdleTimeout(std::chrono::seconds(10));

            this->websocket->control_callback(
                [](
                boost::beast::websocket::frame_type kind,
                boost::beast::string_view
            ) {
                switch (kind) {
                    case boost::beast::websocket::frame_type::pong: {
                        LogDebug("Pong received from websocket");
                        break;
                    }
                    case boost::beast::websocket::frame_type::ping: {
                        LogDebug("Ping received from websocket");
                        break;
                    }
                }
            });
        }

        /*
            Called after connection to the websocket to negotiate a protocol upgrade
        */
        void WebsocketConnection::HandshakeHandler(boost::system::error_code ec)
        {
            if (ec) {
                LogWarning("Websocket: handshake error");
                this->connectionInProgress = false;
                this->ProcessErrorCode(ec);
                return;
            }

            this->connected = true;
            this->connectionInProgress = false;
            this->lastActivityTime = std::chrono::system_clock::now();
            LogInfo("Websocket handshake successful");
        }

        /*
            Loop that runs to send any outgoing messages and read
            any incoming message
        */
        void WebsocketConnection::Loop(void)
        {
            while (this->threadsRunning) {

                std::lock_guard<std::mutex>(this->eventGuard);

                // If not connected, try and connect
                if (!this->connected) {
                    if (!this->connectionInProgress && std::chrono::system_clock::now() > this->nextReconnectAttempt) {
                        this->nextReconnectAttempt = std::chrono::system_clock::now() + this->reconnectAttemptInterval;
                        this->connectionInProgress = true;
                        this->tcpResolver->async_resolve(
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
                    // Sleep for a bit to give the CPU a break
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    continue;
                }

                // Process an inbound message if we can
                if (!this->asyncReadInProgress) {
                    this->asyncReadInProgress = true;
                    this->websocket->async_read(
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
                    this->websocket->async_write(
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

                // This is a mitigation for ongoing websocket issues while we look to fix things,
                // if there's been no activity for a while, reset the websocket
                if (
                    this->connected &&
                    std::chrono::system_clock::now() - this->lastActivityTime > std::chrono::minutes(5)
                ) {
                    LogInfo("Restarting websocket due to inactivity.");
                    boost::beast::error_code ec;

                    try {
                        this->websocket->next_layer().next_layer().cancel();
                        this->websocket->next_layer().next_layer().close();
                        this->websocket->next_layer().shutdown(ec);
                        this->websocket->close(boost::beast::websocket::close_code::normal, ec);
                    } catch (boost::system::system_error) {
                        LogInfo("Exception thrown when trying to close websocket");
                    }

                    this->ResetWebsocket();
                }

                // Sleep for a bit to give the CPU a break
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }

            // Once we get the instruction to terminate, throw away all the incoming messages until we
            // get the close signal, to attain graceful shutdown.
            if (this->connected) {
                boost::system::error_code ec;
                this->websocket->close(boost::beast::websocket::close_code::normal, ec);
            }
        }

        /*
            Function that gets run by a thread to keep the IO Context going
        */
        void WebsocketConnection::RunWebsocket(void)
        {
            this->ioContext.run();
        }

        /*
            Called when a message has been sent by the websocket.
        */
        void WebsocketConnection::MessageSentHandler(boost::system::error_code ec, std::size_t bytes_transferred)
        {
            this->asyncWriteInProgress = false;
            if (ec) {
                LogWarning("Websocket: message sending error");
                this->ProcessErrorCode(ec);
                return;
            }

            this->lastActivityTime = std::chrono::system_clock::now();
        }

        /*
            Called when a message is received. Add it to the inbound message queue.
        */
        void WebsocketConnection::ReadHandler(boost::beast::error_code ec, std::size_t bytes_transferred)
        {
            if (ec) {
                this->asyncReadInProgress = false;
                this->ProcessErrorCode(ec);
                return;
            }

            if (bytes_transferred == 0) {
                this->asyncReadInProgress = false;
                return;
            }

            LogDebug("Incoming websocket message: " + boost::beast::buffers_to_string(this->incomingBuffer.data()));
            std::lock_guard<std::mutex> lock(this->inboundMessageQueueGuard);
            this->inboundMessages.push(buffers_to_string(this->incomingBuffer.data()));
            this->incomingBuffer.consume(bytes_transferred);
            this->asyncReadInProgress = false;
            this->lastActivityTime = std::chrono::system_clock::now();
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
                this->ResetWebsocket();
                this->ProcessErrorCode(ec);
                return;
            }

            boost::beast::get_lowest_layer(*this->websocket).async_connect(
                results.cbegin(),
                results.cend(),
                std::bind(
                    &WebsocketConnection::ConnectHandler,
                    this,
                    std::placeholders::_1
                )
            );
        }

        /*
            Called once the SSL handshake has been performed
        */
        void WebsocketConnection::SSLHandshakeHandler(boost::system::error_code ec)
        {
            if (ec) {
                LogWarning("Websocket: SSL handshake error");
                this->connectionInProgress = false;
                this->ProcessErrorCode(ec);
                return;
            }

            // No timeout required on the stream as websockets handles this.
            boost::beast::get_lowest_layer(*this->websocket).expires_never();

            // Identify the user agent
            this->websocket->set_option(boost::beast::websocket::stream_base::decorator(
                [](boost::beast::websocket::request_type& req) {
                    req.set(
                        boost::beast::http::field::user_agent,
                        std::string(BOOST_BEAST_VERSION_STRING) + " UCKP " + PluginVersion::version
                    );
                }
            ));

            // Perform the websocket handshake
            this->websocket->async_handshake(
                this->host,
                "/app/ukcpwebsocket",
                std::bind(
                    &WebsocketConnection::HandshakeHandler,
                    this,
                    std::placeholders::_1
                )
            );
            LogInfo("Websocket: SSL Handshake Successful");
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
            if (!this->connected) {
                return;
            }
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
            Sets the idle timeout on the websocket
        */
        void WebsocketConnection::SetIdleTimeout(std::chrono::seconds timeout)
        {
            boost::beast::websocket::stream_base::timeout opt{
                std::chrono::seconds(10),
                std::chrono::seconds(timeout),
                true
            };
            this->websocket->set_option(opt);
        }

        /*
            Get the time since last activity
        */
        std::chrono::seconds WebsocketConnection::GetTimeSinceLastActivity(void) const
        {
            return std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now() - this->lastActivityTime
            );
        }

        /*
            Force the websocket to disconnect
        */
        void WebsocketConnection::ForceDisconnect(void)
        {
            std::lock_guard<std::mutex>(this->eventGuard);
            if (!this->connected) {
                return;
            }

            LogInfo("Forcing disconnect from websocket");
            this->websocket->async_close(
                boost::beast::websocket::close_code::normal,
                std::bind(
                    &WebsocketConnection::CloseHandler,
                    this,
                    std::placeholders::_1
                )
            );
        }

        /*
            Processes an error code from the websocket.
        */
        void WebsocketConnection::ProcessErrorCode(boost::system::error_code ec)
        {
            // Handle disconnection
            if (
                ec == boost::asio::error::eof ||
                ec == boost::asio::error::connection_aborted ||
                ec == boost::asio::error::connection_reset ||
                ec == boost::asio::ssl::error::stream_truncated ||
                ec == boost::asio::ssl::error::unexpected_result ||
                ec == boost::asio::ssl::error::unspecified_system_error ||
                ec == boost::beast::error::timeout ||
                ec == boost::beast::websocket::error::closed ||
                ec.value() == 1236  // The network connection was aborted by the local system
            ) {
                this->ResetWebsocket();
                LogWarning("Disconnected from websocket: " + ec.message());
                return;
            }

            // Ignore this code
            if (ec == boost::asio::error::operation_aborted) {
                return;
            }

            LogWarning("Error code from websocket: " + std::to_string(ec.value()) + " (" + ec.message() + ")");
        }
    }  // namespace Websocket
}  // namespace UKControllerPlugin
