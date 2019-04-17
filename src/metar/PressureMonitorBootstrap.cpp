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
            container.metarEventHandler->RegisterHandler(
                    std::make_shared<PressureMonitor>(
                    *container.pluginUserSettingHandler,
                    *container.userMessager
                )
            );
        }
    }  // namespace Metar
}  // namespace UKControllerPlugin
