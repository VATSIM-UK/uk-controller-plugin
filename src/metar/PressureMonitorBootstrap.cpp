#include "pch/stdafx.h"
#include "metar/PressureMonitorBootstrap.h"
#include "metar/PressureMonitor.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Metar::PressureMonitor;

namespace UKControllerPlugin {
    namespace Metar {

        /*
            Create the PressureMonitor and register it for METAR events.
        */
        void PressureMonitorBootstrap(const PersistenceContainer & container)
        {
            std::shared_ptr<PressureMonitor> handler(
                new PressureMonitor(*container.userMessager, *container.activeCallsigns)
            );

            container.metarEventHandler->RegisterHandler(handler);
            container.userSettingHandlers->RegisterHandler(handler);
        }
    }  // namespace Metar
}  // namespace UKControllerPlugin
