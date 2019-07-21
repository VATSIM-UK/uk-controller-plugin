#pragma once
#include "websocket/WebsocketMessage.h"
#include "websocket/WebsocketEventProcessorInterface.h"
#include "websocket/WebsocketProtocolProcessorInterface.h"

namespace UKControllerPlugin {
    namespace Websocket {

        /*
            An interface to be implemented by classes that
            want to process websocket message.
        */
        class WebsocketEventProcessorCollection
        {
            public:

                void AddProcessor(
                    std::shared_ptr<UKControllerPlugin::Websocket::WebsocketEventProcessorInterface> processor
                );
                void AddProtocolProcessor(
                    std::shared_ptr<UKControllerPlugin::Websocket::WebsocketProtocolProcessorInterface> processor
                );
                size_t CountProcessorsForChannel(std::string event) const;
                size_t CountProcessorsForProtocolEvent(std::string event) const;
                std::set<std::string> GetAllSubscriptions(void) const;
                void ProcessEvent(const UKControllerPlugin::Websocket::WebsocketMessage & message) const;
                void ProcessProtocolEvent(const UKControllerPlugin::Websocket::WebsocketMessage & message) const;

            private:

                std::map<
                    std::string,
                    std::set<std::shared_ptr<UKControllerPlugin::Websocket::WebsocketEventProcessorInterface>>
                > channelMap;

                // Maps protocol events to their processors
                std::map<
                    std::string,
                    std::set<std::shared_ptr<UKControllerPlugin::Websocket::WebsocketProtocolProcessorInterface>>
                > protocolEventMap;
        };
    }  // namespace Websocket
}  // namespace UKControllerPlugin
