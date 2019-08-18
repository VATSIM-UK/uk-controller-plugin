#pragma once
#include "websocket/WebsocketEventProcessorInterface.h"
#include "api/ApiInterface.h"
#include "websocket/WebsocketConnectionInterface.h"
#include "websocket/WebsocketEventProcessorCollection.h"
#include "task/TaskRunnerInterface.h"

namespace UKControllerPlugin {
    namespace Websocket {

        class PusherConnectionChannelSubscriptionEventHandler :
            public UKControllerPlugin::Websocket::WebsocketEventProcessorInterface
        {
            public:

                PusherConnectionChannelSubscriptionEventHandler(
                    UKControllerPlugin::Websocket::WebsocketConnectionInterface & websocket,
                    const UKControllerPlugin::Websocket::WebsocketEventProcessorCollection & processors,
                    const UKControllerPlugin::Api::ApiInterface & api,
                    UKControllerPlugin::TaskManager::TaskRunnerInterface & taskRunner
                );

                bool ChannelIsPrivate(std::string channelName) const;
                std::string GetCachedAuthCode(std::string channelName) const;

                const std::map<std::string, std::string> & GetCachedChannelAuthCodes(void) const;

                // Inherited via WebsocketEventProcessorInterface
                void ProcessWebsocketMessage(const UKControllerPlugin::Websocket::WebsocketMessage & message) override;
                std::set<WebsocketSubscription> GetSubscriptions(void) const override;

            private:

                // The websocket connection
                UKControllerPlugin::Websocket::WebsocketConnectionInterface & websocket;

                // Classes that process websocket messages and need to know about events
                const UKControllerPlugin::Websocket::WebsocketEventProcessorCollection & processors;

                // The API, used for authenticating channels
                const UKControllerPlugin::Api::ApiInterface & api;

                // For running async tasks
                UKControllerPlugin::TaskManager::TaskRunnerInterface & taskRunner;

                // Map of channel authorisation codes, so that they may be reused.
                std::map<std::string, std::string> channelAuthCodes;
        };
    }  // namespace UKControllerPlugin
}  // namespace Websocket
