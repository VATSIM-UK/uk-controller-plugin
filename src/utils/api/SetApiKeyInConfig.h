#pragma once
#include "ApiKeyReceivedEvent.h"
#include "eventhandler/EventHandler.h"

namespace UKControllerPlugin::Setting {
    class SettingRepositoryInterface;
} // namespace UKControllerPlugin::Setting

namespace UKControllerPluginUtils::Api {

    class SetApiKeyInConfig : public EventHandler::EventHandler<ApiKeyReceivedEvent>
    {
        public:
        SetApiKeyInConfig(UKControllerPlugin::Setting::SettingRepositoryInterface& settingProvider);
        void OnEvent(const ApiKeyReceivedEvent& event) override;

        private:
        UKControllerPlugin::Setting::SettingRepositoryInterface& settingProvider;
    };
} // namespace UKControllerPluginUtils::Api
