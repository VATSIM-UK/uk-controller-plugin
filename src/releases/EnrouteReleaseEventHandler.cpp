#include "pch/stdafx.h"
#include "releases/EnrouteReleaseEventHandler.h"

using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Websocket::WebsocketMessage;
using UKControllerPlugin::Websocket::WebsocketSubscription;

namespace UKControllerPlugin {
    namespace Releases {

        EnrouteReleaseEventHandler::EnrouteReleaseEventHandler(
            const ApiInterface& api,
            std::set<EnrouteReleaseType, CompareEnrouteReleaseTypes> releaseTypes
        )
            : api(api), releaseTypes(releaseTypes)
        {
        }

        void EnrouteReleaseEventHandler::ProcessWebsocketMessage(const WebsocketMessage& message)
        {

        }

        std::set<WebsocketSubscription> EnrouteReleaseEventHandler::GetSubscriptions(void) const
        {
            return {
                {
                    WebsocketSubscription::SUB_TYPE_CHANNEL,
                    "private-enroute-releases"
                }
            };
        }

        bool EnrouteReleaseEventHandler::ReleaseMessageValid(const nlohmann::json& message) const
        {
            return message.contains("callsign") &&
                message.at("callsign").is_string() &&
                message.contains("type") &&
                message.at("type").is_number_integer() &&
                this->releaseTypes.find(message.at("type").get<int>())
                    != this->releaseTypes.cend() &&
                message.contains("release_point") &&
                (message.at("release_point").is_null() || message.at("release_point").is_string()) &&
                message.contains("initiating_controller") &&
                message.at("initiating_controller").is_string()&&
                message.contains("target_controller") &&
                message.at("target_controller").is_string();
        }
    }  // namespace Releases
}  // namespace UKControllerPlugin
