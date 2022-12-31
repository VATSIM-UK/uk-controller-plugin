#include "CachedAircraftFirExitGenerator.h"
#include "IntentionCodeBootstrapProvider.h"
#include "IntentionCodeModuleFactory.h"
#include "bootstrap/ModuleFactories.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"

namespace UKControllerPlugin::IntentionCode {

    void IntentionCodeBootstrapProvider::BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
        static_cast<void>(container.moduleFactories->IntentionCode().IntentionCodes(*container.dependencyLoader,
                                                                                    *container.activeCallsigns));
        
        container.flightplanHandler->RegisterHandler(
            container.moduleFactories->IntentionCode().CachedFirExitGenerator(*container.dependencyLoader));
    }
} // namespace UKControllerPlugin::IntentionCode
