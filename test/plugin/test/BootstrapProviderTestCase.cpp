#include "BootstrapProviderTestCase.h"
#include "list/PopupListFactory.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/UKPlugin.h"

using UKControllerPlugin::List::PopupListFactory;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;

namespace UKControllerPluginTest {

    BootstrapProviderTestCase::BootstrapProviderTestCase()
        : container(MakeContainer()), displayFactory(*container.pluginFunctionHandlers, configurableDisplays)
    {
    }

    void
    BootstrapProviderTestCase::RunBootstrapPlugin(UKControllerPlugin::Bootstrap::BootstrapProviderInterface& provider)
    {
        provider.BootstrapPlugin(container);
    }

    void BootstrapProviderTestCase::RunBootstrapRadarScreen(
        UKControllerPlugin::Bootstrap::BootstrapProviderInterface& provider)
    {
        provider.BootstrapRadarScreen(container, renderers, configurableDisplays, asrHandlers, displayFactory);
    }

    auto BootstrapProviderTestCase::MakeContainer() -> PersistenceContainer
    {
        PersistenceContainer container;
        container.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
        container.popupListFactory =
            std::make_unique<PopupListFactory>(*container.pluginFunctionHandlers, *container.plugin);
        return container;
    }
} // namespace UKControllerPluginTest
