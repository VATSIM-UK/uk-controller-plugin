#pragma once
#include "websocket/WebsocketMessage.h"
#include "websocket/WebsocketEventProcessorInterface.h"

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
                size_t CountProcessorsForChannel(std::string event) const;
                size_t CountProcessorsForEvent(std::string event) const;
                std::set<std::string> GetChannelSubscriptions(void) const;
                void ProcessEvent(const UKControllerPlugin::Websocket::WebsocketMessage & message) const;

            private:

                std::map<
                    std::string,
                    std::set<std::shared_ptr<UKControllerPlugin::Websocket::WebsocketEventProcessorInterface>>
                > channelMap;

                // Maps protocol events to their processors
                std::map<
                    std::string,
                    std::set<std::shared_ptr<UKControllerPlugin::Websocket::WebsocketEventProcessorInterface>>
                > eventMap;
        };
    }  // namespace Websocket
}  // namespace UKControllerPlugin
