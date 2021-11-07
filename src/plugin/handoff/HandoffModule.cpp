#include "DepartureHandoffResolver.h"
#include "FlightplanSidHandoffMapper.h"
#include "HandoffCache.h"
#include "HandoffCollectionFactory.h"
#include "HandoffEventHandler.h"
#include "HandoffModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "controller/ActiveCallsignCollection.h"
#include "dependency/DependencyLoaderInterface.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "tag/TagItemCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;

namespace UKControllerPlugin::Handoff {

    const int handoffTagItem = 107;

    void BootstrapPlugin(PersistenceContainer& container, DependencyLoaderInterface& dependency)
    {
        std::shared_ptr<HandoffEventHandler> handler = std::make_shared<HandoffEventHandler>(
            std::make_shared<DepartureHandoffResolver>(
                std::make_shared<FlightplanSidHandoffMapper>(
                    Create(
                        *container.controllerPositions,
                        dependency.LoadDependency(GetHandoffDependencyKey(), nlohmann::json::array())),
                    *container.sids),
                *container.activeCallsigns),
            std::make_shared<HandoffCache>(),
            *container.integrationModuleContainer->outboundMessageHandler);

        container.tagHandler->RegisterTagItem(handoffTagItem, handler);
        container.flightplanHandler->RegisterHandler(handler);
        container.activeCallsigns->AddHandler(handler);
    }

    auto GetHandoffDependencyKey() -> std::string
    {
        return "DEPENDENCY_HANDOFF_V2";
    }
} // namespace UKControllerPlugin::Handoff
