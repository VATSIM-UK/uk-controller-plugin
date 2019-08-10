#pragma once
#include "pch/stdafx.h"
#include "websocket/PusherConnectionChannelSubscriptionEventHandler.h"
#include "api/ApiException.h"

using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::TaskManager::TaskRunnerInterface;

namespace UKControllerPlugin {
    namespace Websocket {

        PusherConnectionChannelSubscriptionEventHandler::PusherConnectionChannelSubscriptionEventHandler(
            WebsocketConnectionInterface & websocket,
            const WebsocketEventProcessorCollection & processors,
            const ApiInterface & api,
            TaskRunnerInterface & taskRunner
        )
            : websocket(websocket), processors(processors), api(api), taskRunner(taskRunner)
        {
        }

        const std::map<std::string, std::string>& 
            PusherConnectionChannelSubscriptionEventHandler::GetCachedChannelAuthCodes(void) const
        {
            return this->channelAuthCodes;
        }

        /*
            For each channel that needs to be joined, join it!
        */
        void PusherConnectionChannelSubscriptionEventHandler::ProcessWebsocketMessage(const WebsocketMessage & message)
        {
            std::string socketId = message.data["socket_id"].get<std::string>();

            for (
                std::set<std::string>::const_iterator it = this->processors.GetChannelSubscriptions().cbegin();
                it != this->processors.GetChannelSubscriptions().cend();
                ++it
            ) {
                std::string channel = *it;
                this->taskRunner.QueueAsynchronousTask(
                    [this, socketId, channel](void) {

                        std::string authCode;


                        // Auth codes shouldn't change so reuse the old ones if we can
                        if (this->channelAuthCodes.count(channel)) {
                            authCode = this->channelAuthCodes[channel];
                        } else {
                            try {
                                authCode = this->api.AuthoriseWebsocketChannel(socketId, channel);
                                this->channelAuthCodes[channel] = authCode;
                            } catch (ApiException exception) {
                                LogError(
                                    "Unable to get auth code for channel " + channel
                                    + ": api responded with " + exception.what()
                                );
                                return;
                            }
                        }

                        // Subscribe
                        nlohmann::json subscriptionData;
                        subscriptionData["event"] = "pusher:subscribe";
                        subscriptionData["data"] = {
                            {"channel", channel},
                            {"auth", authCode}
                        };

                        this->websocket.WriteMessage(subscriptionData.dump());
                    }
                );
            }
        }

        /*
            Return the events to subscribe to.
        */
        std::set<WebsocketSubscription> PusherConnectionChannelSubscriptionEventHandler::GetSubscriptions(void) const
        {
            return {
                {
                    WebsocketSubscription::SUB_TYPE_EVENT,
                    "pusher:connection_established"
                }
            };
        }
    }  // namespace UKControllerPlugin
}  // namespace Websocket
