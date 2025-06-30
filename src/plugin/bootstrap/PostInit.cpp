#include "PersistenceContainer.h"
#include "PostInit.h"
#include "countdown/CountdownModule.h"
#include "euroscope/LoadDefaultUserSettings.h"
#include "euroscope/PluginSettingsProviderCollection.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/UKPlugin.h"
#include "tag/TagItemCollection.h"
#include "update/PluginVersion.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Plugin::PluginVersion;

namespace UKControllerPlugin::Bootstrap {

    /*
        Run housekeeping and post-setup processes before we go live.
    */
    void PostInit::Process(const PersistenceContainer& container)
    {
        container.tagHandler->RegisterAllItemsWithEuroscope(*container.plugin);
        container.pluginFunctionHandlers->RegisterTagFunctionsWithEuroscope(*container.plugin);

        UKPlugin& ukPlugin = static_cast<UKPlugin&>(*container.plugin);
        ukPlugin.PostInit();
        UKControllerPlugin::Euroscope::LoadDefaultUserSettings(*container.pluginUserSettingHandler);
        UKControllerPlugin::Countdown::CountdownModule::LoadDefaultUserSettings(*container.pluginUserSettingHandler);
        container.userSettingHandlers->UserSettingsUpdateEvent(*container.pluginUserSettingHandler);
        container.pluginSettingsProviders->Load();

        std::string versionMessage =
            "UK Controller Plugin Loaded Successfully: Version " + std::string(PluginVersion::version);
        ukPlugin.DisplayUserMessage("message", "UKCP", versionMessage.c_str(), false, false, false, false, false);
    }
} // namespace UKControllerPlugin::Bootstrap
