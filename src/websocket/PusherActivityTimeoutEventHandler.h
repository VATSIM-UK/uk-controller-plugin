#pragma once
#include "websocket/WebsocketEventProcessorInterface.h"
#include "websocket/WebsocketConnectionInterface.h"
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin {
    namespace Websocket {


        /*
            A class that handles activity timeouts from the websocket and
            makes sure the connection is well maintained.
        */
        class PusherActivityTimeoutEventHandler :
            public UKControllerPlugin::Websocket::WebsocketEventProcessorInterface,
            public UKControllerPlugin::TimedEvent::AbstractTimedEvent
        {
            public:

                PusherActivityTimeoutEventHandler(
                    UKControllerPlugin::Websocket::WebsocketConnectionInterface & websocket
                );

                // Inherited via WebsocketEventProcessorInterface
                void ProcessWebsocketMessage(const UKControllerPlugin::Websocket::WebsocketMessage & message) override;
                std::set<WebsocketSubscription> GetSubscriptions(void) const override;

                // Inherited via AbstractTimedEvent
                virtual void TimedEventTrigger(void) override;

                // How often should we ping by default
                const std::chrono::seconds defaultPingInterval;

                // How long should we wait for a pong before we kill the connection
                const std::chrono::seconds pongWaitInterval;

            private:

                // Talking to the websocket
                UKControllerPlugin::Websocket::WebsocketConnectionInterface & websocket;

                std::chrono::system_clock::time_point pongTimeout = (std::chrono::system_clock::time_point::max)();

                std::chrono::seconds selectedPingInterval;
        };
    }  // namespace Websocket
}  // namespace UKControllerPlugin
