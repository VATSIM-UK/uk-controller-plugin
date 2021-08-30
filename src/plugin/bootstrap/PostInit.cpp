#include "PersistenceContainer.h"
#include "PostInit.h"
#include "countdown/CountdownModule.h"
#include "euroscope/LoadDefaultUserSettings.h"
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
        container.plugin->PostInit();
        UKControllerPlugin::Euroscope::LoadDefaultUserSettings(*container.pluginUserSettingHandler);
        UKControllerPlugin::Countdown::CountdownModule::LoadDefaultUserSettings(*container.pluginUserSettingHandler);
        container.userSettingHandlers->UserSettingsUpdateEvent(*container.pluginUserSettingHandler);

        std::string versionMessage =
            "UK Controller Plugin Loaded Successfully: Version " + std::string(PluginVersion::version);
        container.plugin->DisplayUserMessage(
            "message", "UKCP", versionMessage.c_str(), false, false, false, false, false);
    }
} // namespace UKControllerPlugin::Bootstrap
