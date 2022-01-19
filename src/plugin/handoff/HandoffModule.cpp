#include "ClearCacheOnActiveCallsignChanges.h"
#include "DepartureHandoffResolver.h"
#include "FlightplanAirfieldHandoffMapper.h"
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

    std::shared_ptr<HandoffCollection> handoffs;                     // NOLINT
    std::shared_ptr<FlightplanSidHandoffMapper> sidMapper;           // NOLINT
    std::shared_ptr<FlightplanAirfieldHandoffMapper> airfieldMapper; // NOLINT
    std::shared_ptr<HandoffCache> cache;                             // NOLINT

    void BootstrapPlugin(PersistenceContainer& container, DependencyLoaderInterface& dependency)
    {
        cache = std::make_shared<HandoffCache>();
        handoffs = Create(
            *container.controllerHierarchyFactory,
            dependency.LoadDependency(GetHandoffDependencyKey(), nlohmann::json::array()));
        airfieldMapper = std::make_shared<FlightplanAirfieldHandoffMapper>(*handoffs, *container.airfields);
        sidMapper = std::make_shared<FlightplanSidHandoffMapper>(*handoffs, *container.sids);

        std::shared_ptr<HandoffEventHandler> handler = std::make_shared<HandoffEventHandler>(
            std::make_shared<DepartureHandoffResolver>(*sidMapper, *airfieldMapper, *container.activeCallsigns),
            cache,
            *container.integrationModuleContainer->outboundMessageHandler);

        container.tagHandler->RegisterTagItem(handoffTagItem, handler);
        container.flightplanHandler->RegisterHandler(handler);
        container.activeCallsigns->AddHandler(std::make_shared<ClearCacheOnActiveCallsignChanges>(*cache));
    }

    auto GetHandoffDependencyKey() -> std::string
    {
        return "DEPENDENCY_HANDOFFS_V2";
    }
} // namespace UKControllerPlugin::Handoff
