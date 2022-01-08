#include "MetarEventHandlerCollection.h"
#include "PressureMonitor.h"
#include "PressureMonitorBootstrap.h"
#include "euroscope/UserSettingAwareCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Metar::PressureMonitor;

namespace UKControllerPlugin::Metar {

    /*
        Create the PressureMonitor and register it for METAR events.
    */
    void PressureMonitorBootstrap(const PersistenceContainer& container)
    {
        std::shared_ptr<PressureMonitor> handler(
            new PressureMonitor(*container.userMessager, *container.activeCallsigns));

        container.metarEventHandler->RegisterHandler(handler);
        container.userSettingHandlers->RegisterHandler(handler);
    }
} // namespace UKControllerPlugin::Metar
