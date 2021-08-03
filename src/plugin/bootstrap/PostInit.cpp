#include "pch/pch.h"
#include "bootstrap/PostInit.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/LoadDefaultUserSettings.h"
#include "countdown/CountdownModule.h"
#include "update/PluginVersion.h"

using UKControllerPlugin::Plugin::PluginVersion;
using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin {
    namespace Bootstrap {

        /*
            Run housekeeping and post-setup processes before we go live.
        */
        void PostInit::Process(const PersistenceContainer & container)
        {
            container.tagHandler->RegisterAllItemsWithEuroscope(*container.plugin);
            container.pluginFunctionHandlers->RegisterTagFunctionsWithEuroscope(*container.plugin);
            container.plugin->PostInit();
            UKControllerPlugin::Euroscope::LoadDefaultUserSettings(*container.pluginUserSettingHandler);
            UKControllerPlugin::Countdown::CountdownModule::LoadDefaultUserSettings(
                *container.pluginUserSettingHandler
            );
            container.userSettingHandlers->UserSettingsUpdateEvent(*container.pluginUserSettingHandler);

            std::string versionMessage = "UK Controller Plugin Loaded Successfully: Version " +
                std::string(PluginVersion::version);
            container.plugin->DisplayUserMessage(
                "message",
                "UKCP",
                versionMessage.c_str(),
                false,
                false,
                false,
                false,
                false
            );
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
