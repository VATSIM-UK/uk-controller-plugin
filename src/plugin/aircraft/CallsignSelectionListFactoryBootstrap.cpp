#include "CallsignSelectionListFactoryBootstrap.h"
#include "CallsignSelectionListFactory.h"
#include "bootstrap/PersistenceContainer.h"
#include "plugin/UKPlugin.h"

namespace UKControllerPlugin::Aircraft {
    void BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
        container.callsignSelectionListFactory =
            std::make_unique<CallsignSelectionListFactory>(*container.pluginFunctionHandlers, *container.plugin);
    }
} // namespace UKControllerPlugin::Aircraft
