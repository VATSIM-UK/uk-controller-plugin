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

        void EnrouteReleaseEventHandler::AddIncomingRelease(std::string callsign, EnrouteRelease release)
        {
            this->incomingReleases[callsign] = release;
        }

        void EnrouteReleaseEventHandler::AddOutgoingRelease(std::string callsign, EnrouteRelease release)
        {
            this->outgoingReleases[callsign] = release;
        }

        const EnrouteRelease& EnrouteReleaseEventHandler::GetIncomingRelease(std::string callsign) const
        {
            if (!this->incomingReleases.count(callsign)) {
                return this->invalidRelease;
            }

            return this->incomingReleases.at(callsign);
        }

        const EnrouteRelease& EnrouteReleaseEventHandler::GetOutgoingRelease(std::string callsign) const
        {
            if (!this->outgoingReleases.count(callsign)) {
                return this->invalidRelease;
            }

            return this->outgoingReleases.at(callsign);
        }

        const std::set<EnrouteReleaseType, CompareEnrouteReleaseTypes>
            EnrouteReleaseEventHandler::GetReleaseTypes(void) const
        {
            return this->releaseTypes;
        }

        void EnrouteReleaseEventHandler::ProcessWebsocketMessage(const WebsocketMessage& message)
        {
            if (message.event != "App\\Events\\EnrouteReleaseEvent") {
                return;
            }

            if (!this->ReleaseMessageValid(message.data)) {
                LogWarning("Invalid enroute release message");
                return;
            }

            // Add the incoming release
            this->incomingReleases[message.data.at("callsign").get<std::string>()] = {
                message.data.at("type").get<int>(),
                !message.data.at("release_point").is_null(),
                message.data.at("release_point").is_null() ? "" : message.data.at("release_point").get<std::string>(),
                std::chrono::system_clock::now() + std::chrono::minutes(3)
            };
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

        std::string EnrouteReleaseEventHandler::GetTagItemDescription(int tagItemId) const
        {
            return tagItemId == this->enrouteReleasePointTagItemId
                ? "Enroute Release Point"
                : "Enroute Release Type";
        }

        void EnrouteReleaseEventHandler::SetTagItemData(UKControllerPlugin::Tag::TagData& tagData)
        {
            tagData.SetItemString("");
            if (tagData.itemCode == this->enrouteReleaseTypeTagItemId) {

                // We can skip for valid release type checks here as this is checked on incoming messages
                // Prioritise displaying outgoing releases
                if (this->outgoingReleases.count(tagData.flightPlan.GetCallsign())) {
                    const EnrouteReleaseType & releaseType = *this->releaseTypes.find(
                        this->outgoingReleases.at(tagData.flightPlan.GetCallsign()).releaseType
                    );

                    tagData.SetItemString(releaseType.tagString);
                    tagData.SetTagColour(this->outgoingItemColour);
                } else if (this->incomingReleases.count(tagData.flightPlan.GetCallsign())) {
                    const EnrouteReleaseType& releaseType = *this->releaseTypes.find(
                        this->incomingReleases.at(tagData.flightPlan.GetCallsign()).releaseType
                    );

                    tagData.SetItemString(releaseType.tagString);
                    tagData.SetTagColour(this->incomingItemColour);
                }
            } else if (tagData.itemCode == this->enrouteReleasePointTagItemId) {
                // Prioritise displaying outgoing releases
                if (this->outgoingReleases.count(tagData.flightPlan.GetCallsign())) {
                    if (!this->outgoingReleases.at(tagData.flightPlan.GetCallsign()).hasReleasePoint) {
                        return;
                    }

                    tagData.SetItemString(this->outgoingReleases.at(tagData.flightPlan.GetCallsign()).releasePoint);
                    tagData.SetTagColour(this->outgoingItemColour);
                }
                else if (this->incomingReleases.count(tagData.flightPlan.GetCallsign())) {
                    if (!this->incomingReleases.at(tagData.flightPlan.GetCallsign()).hasReleasePoint) {
                        return;
                    }

                    tagData.SetItemString(this->incomingReleases.at(tagData.flightPlan.GetCallsign()).releasePoint);
                    tagData.SetTagColour(this->incomingItemColour);
                }
            }
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
