#pragma once
#include "pch/stdafx.h"
#include "websocket/PusherConnectionChannelSubscriptionEventHandler.h"
#include "api/ApiException.h"
#include "api/ApiNotAuthorisedException.h"

using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Api::ApiNotAuthorisedException;
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

        bool PusherConnectionChannelSubscriptionEventHandler::ChannelIsPrivate(std::string channelName) const
        {
            return channelName.substr(0, 8) == "private-";
        }

        std::string PusherConnectionChannelSubscriptionEventHandler::GetCachedAuthCode(std::string channelName) const
        {
            return this->channelAuthCodes.count(channelName) ? this->channelAuthCodes.at(channelName) : "";
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
            std::set<std::string> channelSubs = this->processors.GetChannelSubscriptions();


            for (std::set<std::string>::const_iterator it = channelSubs.cbegin(); it != channelSubs.cend(); ++it) {
                std::string channel = *it;
                this->taskRunner.QueueAsynchronousTask(
                    [this, socketId, channel](void) {
                        
                        // Websocket data
                        nlohmann::json subscriptionData;
                        subscriptionData["event"] = "pusher:subscribe";
                        subscriptionData["data"] = {
                            {"channel", channel}
                        };
                        

                        // If the channel is private, get an auth code for it
                        if (this->ChannelIsPrivate(channel)) {
                            std::string authCode;
                            // Auth codes shouldn't change so reuse the old ones if we can
                            if (this->channelAuthCodes.count(channel)) {
                                authCode = this->channelAuthCodes[channel];
                            }
                            else {
                                try {
                                    authCode = this->api.AuthoriseWebsocketChannel(socketId, channel);
                                    this->channelAuthCodes[channel] = authCode;
                                } catch (ApiNotAuthorisedException) {
                                    LogError(
                                        "Unable to get auth code for channel " + channel + ". Not authorised."

                                    );
                                    return;
                                }
                                catch (ApiException exception) {
                                    LogError(
                                        "Unable to get auth code for channel " + channel
                                        + ": api responded with " + exception.what()
                                    );
                                    return;
                                }
                            }

                            subscriptionData["data"].push_back({ "auth", authCode });
                        }

                        // Subscribe
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
