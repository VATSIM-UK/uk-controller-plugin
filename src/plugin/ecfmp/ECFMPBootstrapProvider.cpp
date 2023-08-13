#include "AircraftFlowMeasureMap.h"
#include "AircraftFlowMeasureTagItem.h"
#include "ECFMPBootstrapProvider.h"
#include "ECFMPModuleFactory.h"
#include "TriggerEcfmpEventLoop.h"
#include "bootstrap/ModuleFactories.h"
#include "bootstrap/PersistenceContainer.h"
#include "tag/TagItemCollection.h"
#include "timedevent/TimedEventCollection.h"

namespace UKControllerPlugin::ECFMP {

    void ECFMPBootstrapProvider::BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
        // Event to trigger ECFMP event loop every second
        const auto ecfmpSdk = container.moduleFactories->ECFMP().Sdk(*container.curl);
        container.timedHandler->RegisterEvent(std::make_shared<TriggerECFMPEventLoop>(ecfmpSdk), 1);

        // Tag item to display flow measure for a given aircraft
        const auto aircraftFlowMeasureMap = std::make_shared<AircraftFlowMeasureMap>(*container.plugin);
        container.tagHandler->RegisterTagItem(
            131, std::make_shared<AircraftFlowMeasureTagItem>(aircraftFlowMeasureMap));
    }
} // namespace UKControllerPlugin::ECFMP
