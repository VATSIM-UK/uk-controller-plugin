#include "pch/stdafx.h"
#include "datablock/DatablockBoostrap.h"
#include "datablock/DisplayTime.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin {
    namespace Datablock {

        /*
            Set up the time formatter
        */
        void BootstrapPlugin(PersistenceContainer & container)
        {
            container.timeFormatting = std::make_shared<DisplayTime>();
            container.userSettingHandlers->RegisterHandler(container.timeFormatting);
        }
    }  // namespace Datablock
}  // namespace UKControllerPlugin
