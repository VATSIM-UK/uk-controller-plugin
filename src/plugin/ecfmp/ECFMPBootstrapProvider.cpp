#include "ECFMPBootstrapProvider.h"
#include "ECFMPModuleFactory.h"
#include "TriggerEcfmpEventLoop.h"
#include "bootstrap/ModuleFactories.h"
#include "bootstrap/PersistenceContainer.h"
#include "timedevent/TimedEventCollection.h"

namespace UKControllerPlugin::ECFMP {

    void ECFMPBootstrapProvider::BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
        container.timedHandler->RegisterEvent(
            std::make_shared<TriggerECFMPEventLoop>(container.moduleFactories->ECFMP().Sdk(*container.curl)), 1);
    }
} // namespace UKControllerPlugin::ECFMP
