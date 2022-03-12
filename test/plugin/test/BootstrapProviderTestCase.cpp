#include "BootstrapProviderTestCase.h"
#include "plugin/FunctionCallEventHandler.h"

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
        return container;
    }
} // namespace UKControllerPluginTest
