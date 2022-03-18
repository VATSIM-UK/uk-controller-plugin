#include "PluginSettingsProviderCollection.h"
#include "PluginSettingsProviderInterface.h"

namespace UKControllerPlugin::Euroscope {

    PluginSettingsProviderCollection::PluginSettingsProviderCollection(UserSetting& settings) : settings(settings)
    {
    }

    void PluginSettingsProviderCollection::AddProvider(std::shared_ptr<PluginSettingsProviderInterface> provider)
    {
        auto noExisting = std::find(providers.begin(), providers.end(), provider) == providers.cend();
        assert(noExisting && "Duplicate PluginSettingsProvider added");
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
