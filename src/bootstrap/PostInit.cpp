#include "pch/stdafx.h"
#include "bootstrap/PostInit.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/LoadDefaultUserSettings.h"

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
            container.userSettingHandlers->UserSettingsUpdateEvent(*container.pluginUserSettingHandler);
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
