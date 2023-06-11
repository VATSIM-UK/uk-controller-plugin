#include "DepartureHandoffIntegrationDataInitialiser.h"
#include "DepartureHandoffResolvedEvent.h"
#include "DefaultDepartureHandoffResolutionStrategy.h"
#include "DefaultDepartureHandoffResolver.h"
#include "FlightplanAirfieldHandoffMapper.h"
#include "FlightplanSidHandoffMapper.h"
#include "HandoffCollectionFactory.h"
#include "HandoffEventHandler.h"
#include "HandoffModule.h"
#include "InvalidateHandoffsOnActiveCallsignChanges.h"
#include "InvalidateHandoffsOnRunwayDialogSave.h"
#include "SendHandoffFrequencyUpdatedIntegrationMessage.h"
#include "bootstrap/PersistenceContainer.h"
#include "controller/ActiveCallsignCollection.h"
#include "dependency/DependencyLoaderInterface.h"
#include "euroscope/RunwayDialogAwareCollection.h"
#include "eventhandler/EventBus.h"
#include "eventhandler/EventHandlerFlags.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "integration/IntegrationPersistenceContainer.h"
#include "integration/IntegrationDataInitialisers.h"
#include "tag/TagItemCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;

namespace UKControllerPlugin::Handoff {

    const int handoffTagItem = 107;

    std::shared_ptr<HandoffCollection> handoffs;                     // NOLINT
    std::shared_ptr<FlightplanSidHandoffMapper> sidMapper;           // NOLINT
    std::shared_ptr<FlightplanAirfieldHandoffMapper> airfieldMapper; // NOLINT

    void BootstrapPlugin(PersistenceContainer& container, DependencyLoaderInterface& dependency)
    {
        handoffs = Create(
            *container.controllerHierarchyFactory,
            dependency.LoadDependency(GetHandoffDependencyKey(), nlohmann::json::array()));
        airfieldMapper = std::make_shared<FlightplanAirfieldHandoffMapper>(*handoffs, *container.airfields);
        sidMapper = std::make_shared<FlightplanSidHandoffMapper>(*handoffs, *container.sidMapper);

        // Create the default resolution strategy
        auto strategy = std::make_unique<DefaultDepartureHandoffResolutionStrategy>(
            *sidMapper, *airfieldMapper, *container.activeCallsigns);

        // Create the default resolver using the strategy and add to the container
        std::shared_ptr<DefaultDepartureHandoffResolver> resolver =
            std::make_shared<DefaultDepartureHandoffResolver>(std::move(strategy));
        container.departureHandoffResolver = resolver;

        // Create the event handler
        std::shared_ptr<HandoffEventHandler> handler = std::make_shared<HandoffEventHandler>(resolver);

        container.tagHandler->RegisterTagItem(handoffTagItem, handler);
        container.flightplanHandler->RegisterHandler(handler);
        container.activeCallsigns->AddHandler(
            std::make_shared<InvalidateHandoffsOnActiveCallsignChanges>(resolver, *container.plugin));
        container.runwayDialogEventHandlers->AddHandler(
            std::make_shared<InvalidateHandoffsOnRunwayDialogSave>(resolver, *container.plugin));

        // Integration data initialiser
        container.integrationModuleContainer->dataInitialisers->Add(
            std::make_shared<DepartureHandoffIntegrationDataInitialiser>(resolver, *container.plugin));

        // Event handler for sending frequency updates to integrations
        UKControllerPluginUtils::EventHandler::EventBus::Bus().AddHandler<DepartureHandoffResolvedEvent>(
            std::make_shared<SendHandoffFrequencyUpdatedIntegrationMessage>(
                *container.integrationModuleContainer->outboundMessageHandler),
            UKControllerPluginUtils::EventHandler::EventHandlerFlags::Sync);
    }

    auto GetHandoffDependencyKey() -> std::string
    {
        return "DEPENDENCY_HANDOFFS_V2";
    }
} // namespace UKControllerPlugin::Handoff
