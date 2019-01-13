#include "pch/stdafx.h"
#include "initialaltitude/InitialAltitudeModule.h"
#include "initialaltitude/InitialAltitudeEventHandler.h"
#include "initialaltitude/InitialAltitudeGeneratorFactory.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "bootstrap/PersistenceContainer.h"

using UKControllerPlugin::InitialAltitude::InitialAltitudeGeneratorFactory;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::InitialAltitude::InitialAltitudeEventHandler;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
namespace UKControllerPlugin {
    namespace InitialAltitude {

        /*
            Initialises the initial altitude module. Gets the altitudes from the dependency cache
            and registers the event handler to receive flightplan events.
        */
        void InitialAltitudeModule::BootstrapPlugin(
            DependencyCache & dependency,
            PersistenceContainer & persistence
        ) {
            persistence.initialAltitudes = InitialAltitudeGeneratorFactory::Create(dependency);
            std::shared_ptr<InitialAltitudeEventHandler> initialAltitudeEventHandler(
                new InitialAltitudeEventHandler(
                    *persistence.initialAltitudes,
                    *persistence.activeCallsigns,
                    *persistence.airfieldOwnership,
                    *persistence.login,
                    *persistence.deferredHandlers,
                    *persistence.plugin
                )
            );

            persistence.initialAltitudeEvents = initialAltitudeEventHandler;
            persistence.flightplanHandler->RegisterHandler(initialAltitudeEventHandler);
        }
    }  // namespace InitialAltitude
}  // namespace UKControllerPlugin
