#include "pch/stdafx.h"
#include "releases/DepartureReleaseEventHandler.h"
#include "releases/DepartureReleaseRequest.h"
#include "time/ParseTimeStrings.h"

namespace UKControllerPlugin {
    namespace Releases {

        void DepartureReleaseEventHandler::ProcessWebsocketMessage(const Websocket::WebsocketMessage& message)
        {
            if (message.event == "departure_release.requested") {
                this->ProcessDepartureReleaseRequestedMessage(message.data);
            } else if (message.event == "departure_release.approved") {
                this->ProcessRequestApprovedMessage(message.data);
            } else if (message.event == "departure_release.rejected") {
                this->ProcessRequestRejectedMessage(message.data);
            } else if (message.event == "departure_release.acknowledged") {
                this->ProcessRequestAcknowledgedMessage(message.data);
            } else if (message.event == "departure_release.request_cancelled") {
                this->ProcessRequestCancelledMessage(message.data);
            }
        }

        std::set<Websocket::WebsocketSubscription> DepartureReleaseEventHandler::GetSubscriptions() const
        {
            return {
                {
                    Websocket::WebsocketSubscription::SUB_TYPE_CHANNEL,
                    "private-departure-releases"
                }
            };
        }

        void DepartureReleaseEventHandler::AddReleaseRequest(std::shared_ptr<DepartureReleaseRequest> request)
        {
            this->releaseRequests[request->Id()] = request;
        }

        std::shared_ptr<DepartureReleaseRequest> DepartureReleaseEventHandler::GetReleaseRequest(int id) const
        {
            auto request = this->releaseRequests.find(id);
            return request == this->releaseRequests.cend() ? nullptr : request->second;
        }

        bool DepartureReleaseEventHandler::DepartureReleaseRequestedMessageValid(const nlohmann::json& data)
        {
            //TODO: Check Target/Requesting is valid controller
            return data.is_object() &&
                data.contains("id") &&
                data.at("id").is_number_integer() &&
                data.contains("callsign") &&
                data.at("callsign").is_string() &&
                data.contains("requesting_controller") &&
                data.at("requesting_controller").is_number_integer() &&
                data.contains("target_controller") &&
                data.at("target_controller").is_number_integer() &&
                data.contains("expires_at") &&
                data.at("expires_at").is_string() &&
                Time::ParseTimeString(data.at("expires_at").get<std::string>()) != Time::invalidTime;
        }

        /*
         * An acknowledged message needs an id that we know about.
         */
        bool DepartureReleaseEventHandler::DepartureReleaseAcknowledgedMessageValid(const nlohmann::json& data) const
        {
            return data.is_object() &&
                data.contains("id") &&
                data.at("id").is_number_integer() &&
                this->releaseRequests.find(data.at("id").get<int>()) != this->releaseRequests.cend();
        }

        /*
         * For now, rejecting a release only needs a valid id, so borrow the acknowledge method.
         */
        bool DepartureReleaseEventHandler::DepartureReleaseRejectedMessageValid(const nlohmann::json& data) const
        {
            return this->DepartureReleaseAcknowledgedMessageValid(data);
        }

        bool DepartureReleaseEventHandler::DepartureReleaseApprovedMessageValid(const nlohmann::json& data) const
        {
            return data.is_object() &&
                data.contains("id") &&
                data.at("id").is_number_integer() &&
                this->releaseRequests.find(data.at("id").get<int>()) != this->releaseRequests.cend() &&
                data.contains("expires_at") &&
                data.at("expires_at").is_string() &&
                Time::ParseTimeString(data.at("expires_at").get<std::string>()) != Time::invalidTime &&
                data.contains("released_at") &&
                data.at("released_at").is_string() &&
                Time::ParseTimeString(data.at("released_at").get<std::string>()) != Time::invalidTime;
        }

        /*
         * For now, cancelling a release request only needs a valid id, so borrow the acknowledge method.
         */
        bool DepartureReleaseEventHandler::DepartureReleaseCancelMessageValid(const nlohmann::json& data) const
        {
            return this->DepartureReleaseAcknowledgedMessageValid(data);
        }

        /**
         * Create a new departure release request.
         */
        void DepartureReleaseEventHandler::ProcessDepartureReleaseRequestedMessage(const nlohmann::json& data)
        {
            if (!DepartureReleaseRequestedMessageValid(data)) {
                LogError("Invalid departure release requested message");
                return;
            }

            this->releaseRequests[data.at("id").get<int>()] = std::make_shared<DepartureReleaseRequest>(
                data.at("id").get<int>(),
                data.at("callsign").get<std::string>(),
                data.at("requesting_controller").get<int>(),
                data.at("target_controller").get<int>(),
                Time::ParseTimeString(data.at("expires_at").get<std::string>())
            );
        }

        /**
         * Acknowledge a departure release request.
         */
        void DepartureReleaseEventHandler::ProcessRequestAcknowledgedMessage(const nlohmann::json& data)
        {
            if (!DepartureReleaseAcknowledgedMessageValid(data)) {
                LogError("Invalid departure release acknowledged message");
                return;
            }

            this->releaseRequests.find(data.at("id").get<int>())->second->Acknowledge();
        }

        /**
         * Reject a departure release request.
         */
        void DepartureReleaseEventHandler::ProcessRequestRejectedMessage(const nlohmann::json& data)
        {
            if (!DepartureReleaseRejectedMessageValid(data)) {
                LogError("Invalid departure release rejected message");
                return;
            }

            this->releaseRequests.find(data.at("id").get<int>())->second->Reject();
        }

        /**
         * Approve a departure release request.
         */
        void DepartureReleaseEventHandler::ProcessRequestApprovedMessage(const nlohmann::json& data)
        {
            if (!DepartureReleaseApprovedMessageValid(data)) {
                LogError("Invalid departure release approved message");
                return;
            }

            this->releaseRequests.find(data.at("id").get<int>())->second->Approve(
                Time::ParseTimeString(data.at("released_at").get<std::string>()),
                Time::ParseTimeString(data.at("expires_at").get<std::string>())
            );
        }

        /**
         * Cancel a departure release request.
         */
        void DepartureReleaseEventHandler::ProcessRequestCancelledMessage(const nlohmann::json& data)
        {
            if (!DepartureReleaseCancelMessageValid(data)) {
                LogError("Invalid departure release cancelled message");
                return;
            }

            this->releaseRequests.erase(data.at("id").get<int>());
        }
    }  // namespace Releases
}  // namespace UKControllerPlugin
