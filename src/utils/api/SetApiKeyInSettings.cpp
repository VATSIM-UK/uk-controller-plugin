#include "ApiSettings.h"
#include "SetApiKeyInSettings.h"

namespace UKControllerPluginUtils::Api {

    SetApiKeyInSettings::SetApiKeyInSettings(ApiSettings& settings) : settings(settings)
    {
    }

    void SetApiKeyInSettings::OnEvent(const ApiKeyReceivedEvent& event)
    {
        settings.Key(event.key);
    }
} // namespace UKControllerPluginUtils::Api
