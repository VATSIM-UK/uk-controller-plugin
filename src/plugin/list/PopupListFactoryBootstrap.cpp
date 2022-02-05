#include "PopupListFactory.h"
#include "PopupListFactoryBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "plugin/UKPlugin.h"

namespace UKControllerPlugin::List {

    void BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
        container.popupListFactory =
            std::make_unique<PopupListFactory>(*container.pluginFunctionHandlers, *container.plugin);
    }
} // namespace UKControllerPlugin::List
