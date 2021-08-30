#include "DatablockBoostrap.h"
#include "DisplayTime.h"
#include "euroscope/UserSettingAwareCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin::Datablock {

    /*
        Set up the time formatter
    */
    void BootstrapPlugin(PersistenceContainer& container)
    {
        container.timeFormatting = std::make_shared<DisplayTime>();
        container.userSettingHandlers->RegisterHandler(container.timeFormatting);
    }
} // namespace UKControllerPlugin::Datablock
