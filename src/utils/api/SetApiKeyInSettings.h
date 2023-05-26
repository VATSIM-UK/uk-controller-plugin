#pragma once
#include "ApiKeyReceivedEvent.h"
#include "eventhandler/EventHandler.h"

namespace UKControllerPluginUtils::Api {

    class ApiSettings;

    class SetApiKeyInSettings : public EventHandler::EventHandler<ApiKeyReceivedEvent>
    {
        public:
        SetApiKeyInSettings(ApiSettings& settings);
        void OnEvent(const ApiKeyReceivedEvent& event) override;

        private:
        ApiSettings& settings;
    };
} // namespace UKControllerPluginUtils::Api
