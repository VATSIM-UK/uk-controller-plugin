#include "BootstrapProviderTestCase.h"
#include "aircraft/CallsignSelectionListFactory.h"
#include "bootstrap/ModuleBootstrap.h"
#include "controller/ActiveCallsignCollection.h"
#include "list/PopupListFactory.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/UKPlugin.h"
#include "tag/TagItemCollection.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Aircraft::CallsignSelectionListFactory;
using UKControllerPlugin::Bootstrap::ModuleBootstrap;
using UKControllerPlugin::List::PopupListFactory;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;

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
        ModuleBootstrap(container);
        container.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
        container.popupListFactory =
            std::make_unique<PopupListFactory>(*container.pluginFunctionHandlers, *container.plugin);
        container.callsignSelectionListFactory =
            std::make_unique<CallsignSelectionListFactory>(*container.popupListFactory);
        container.tagHandler = std::make_unique<TagItemCollection>();
        container.timedHandler = std::make_unique<TimedEventCollection>();
        container.activeCallsigns = std::make_unique<UKControllerPlugin::Controller::ActiveCallsignCollection>();
        return container;
    }
} // namespace UKControllerPluginTest
