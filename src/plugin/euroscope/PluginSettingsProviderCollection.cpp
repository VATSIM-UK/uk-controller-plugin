#include "PluginSettingsProviderCollection.h"
#include "PluginSettingsProviderInterface.h"

namespace UKControllerPlugin::Euroscope {

    PluginSettingsProviderCollection::PluginSettingsProviderCollection(UserSetting& settings) : settings(settings)
    {
    }

    void PluginSettingsProviderCollection::AddProvider(std::shared_ptr<PluginSettingsProviderInterface> provider)
    {
        if (std::find(providers.begin(), providers.end(), provider) != providers.cend()) {
            LogError("Duplicate plugin settings provider added");
            return;
        }

        providers.push_back(provider);
    }

    void PluginSettingsProviderCollection::Load() const
    {
        for (const auto& provider : providers) {
            provider->LoadSettings(settings);
        }
    }

    void PluginSettingsProviderCollection::Save() const
    {
        for (const auto& provider : providers) {
            provider->SaveSettings(settings);
        }
    }

    auto PluginSettingsProviderCollection::Count() const -> size_t
    {
        return providers.size();
    }
} // namespace UKControllerPlugin::Euroscope
