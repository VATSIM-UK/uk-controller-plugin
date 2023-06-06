#pragma once
#include "eventhandler/EventHandler.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
    class UserSetting;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Releases {

    class DepartureReleaseRelevanceChecker;
    struct DepartureReleaseRequestedEvent;

    class SendReleaseRequestedChatAreaMessage
        : public UKControllerPluginUtils::EventHandler::EventHandler<DepartureReleaseRequestedEvent>
    {
        public:
        SendReleaseRequestedChatAreaMessage(
            std::shared_ptr<const DepartureReleaseRelevanceChecker> relevanceChecker,
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            Euroscope::UserSetting& userSettings);
        void OnEvent(const DepartureReleaseRequestedEvent& event) override;

        private:
        // Checks prenote relevance
        const std::shared_ptr<const DepartureReleaseRelevanceChecker> relevanceChecker;

        // For sending messages
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // For checking user settings
        Euroscope::UserSetting& userSettings;
    };

} // namespace UKControllerPlugin::Releases
