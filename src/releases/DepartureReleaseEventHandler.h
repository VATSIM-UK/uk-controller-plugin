#pragma once
#include "websocket/WebsocketEventProcessorInterface.h"

namespace UKControllerPlugin {
    namespace Releases {

        class DepartureReleaseRequest;

        /*
            Handles events relating to departure releases.
        */
        class DepartureReleaseEventHandler : public Websocket::WebsocketEventProcessorInterface
        {
            public:
                void ProcessWebsocketMessage(const Websocket::WebsocketMessage& message) override;
                std::set<Websocket::WebsocketSubscription> GetSubscriptions() const override;
                void AddReleaseRequest(std::shared_ptr<DepartureReleaseRequest> request);
                std::shared_ptr<DepartureReleaseRequest> GetReleaseRequest(int id) const;

            private:
                void ProcessDepartureReleaseRequestedMessage(const nlohmann::json& data);
                void ProcessRequestAcknowledgedMessage(const nlohmann::json& data);
                void ProcessRequestRejectedMessage(const nlohmann::json& data);
                void ProcessRequestApprovedMessage(const nlohmann::json& data);
                void ProcessRequestCancelledMessage(const nlohmann::json& data);

                static bool DepartureReleaseRequestedMessageValid(const nlohmann::json& data);
                bool DepartureReleaseAcknowledgedMessageValid(const nlohmann::json& data) const;
                bool DepartureReleaseRejectedMessageValid(const nlohmann::json& data) const;
                bool DepartureReleaseApprovedMessageValid(const nlohmann::json& data) const;
                bool DepartureReleaseCancelMessageValid(const nlohmann::json& data) const;

                std::map<int, std::shared_ptr<DepartureReleaseRequest>> releaseRequests;
        };
    }  // namespace Releases
}  // namespace UKControllerPlugin
