#pragma once
#include "websocket/WebsocketEventProcessorInterface.h"
#include "api/ApiInterface.h"
#include "releases/EnrouteReleaseType.h"
#include "releases/CompareEnrouteReleaseTypes.h"

namespace UKControllerPlugin {
    namespace Releases {

        /*
            Handles events around enroute releases
        */
        class EnrouteReleaseEventHandler: public UKControllerPlugin::Websocket::WebsocketEventProcessorInterface
        {
            public:
                EnrouteReleaseEventHandler(
                    const UKControllerPlugin::Api::ApiInterface& api,
                    const std::set<
                        UKControllerPlugin::Releases::EnrouteReleaseType,
                        UKControllerPlugin::Releases::CompareEnrouteReleaseTypes
                    > releaseTypes
                );
                bool ReleaseMessageValid(const nlohmann::json& message) const;

                // Inherited via WebsocketEventProcessorInterface
                void ProcessWebsocketMessage(const UKControllerPlugin::Websocket::WebsocketMessage& message) override;
                std::set<UKControllerPlugin::Websocket::WebsocketSubscription> GetSubscriptions(void) const override;

            private:

                // For sending releases
                const UKControllerPlugin::Api::ApiInterface& api;

                // The different type of enroute releases
                const std::set<
                    UKControllerPlugin::Releases::EnrouteReleaseType,
                    UKControllerPlugin::Releases::CompareEnrouteReleaseTypes
                > releaseTypes;
        };
    }  // namespace Releases
}  // namespace UKControllerPlugin
