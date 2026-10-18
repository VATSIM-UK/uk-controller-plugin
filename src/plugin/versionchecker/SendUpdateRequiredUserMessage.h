#pragma once

#include "UpdateRequiredEvent.h"
#include "eventhandler/EventHandler.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::VersionChecker {

    class SendUpdateRequiredUserMessage
        : public UKControllerPluginUtils::EventHandler::EventHandler<UpdateRequiredEvent>
    {
        public:
        SendUpdateRequiredUserMessage(Euroscope::EuroscopePluginLoopbackInterface& plugin);
        void OnEvent(const UpdateRequiredEvent& event) override;

        private:
        Euroscope::EuroscopePluginLoopbackInterface& plugin;
    };

} // namespace UKControllerPlugin::VersionChecker
