#include "HandoffCollectionFactory.h"
#include "HandoffEventHandler.h"
#include "HandoffModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "tag/TagItemCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;

namespace UKControllerPlugin::Handoff {

    const int handoffTagItem = 107;

    void BootstrapPlugin(PersistenceContainer& container, DependencyLoaderInterface& dependency)
    {
        container.handoffs = Create(
            *container.controllerPositions,
            dependency.LoadDependency(GetHandoffDependencyKey(), "{}"_json),
            dependency.LoadDependency(GetSidHandoffMappingDependencyKey(), "{}"_json));

        std::shared_ptr<HandoffEventHandler> handler = std::make_shared<HandoffEventHandler>(
            *container.handoffs,
            *container.activeCallsigns,
            *container.integrationModuleContainer->outboundMessageHandler);

        container.tagHandler->RegisterTagItem(handoffTagItem, handler);
        container.flightplanHandler->RegisterHandler(handler);
        container.activeCallsigns->AddHandler(handler);
    }

    auto GetHandoffDependencyKey() -> std::string
    {
        return "DEPENDENCY_HANDOFF";
    }

    auto GetSidHandoffMappingDependencyKey() -> std::string
    {
        return "DEPENDENCY_SID_HANDOFF";
    }
} // namespace UKControllerPlugin::Handoff
