#include "pch/stdafx.h"
#include "initialaltitude/InitialAltitudeModule.h"
#include "initialaltitude/InitialAltitudeEventHandler.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "bootstrap/PersistenceContainer.h"
#include "tag/TagFunction.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::InitialAltitude::InitialAltitudeEventHandler;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Tag::TagFunction;
namespace UKControllerPlugin {
    namespace InitialAltitude {

        /*
            Initialises the initial altitude module. Gets the altitudes from the dependency cache
            and registers the event handler to receive flightplan events.
        */
        void InitialAltitudeModule::BootstrapPlugin(
            PersistenceContainer& persistence
        ) {
            std::shared_ptr<InitialAltitudeEventHandler> initialAltitudeEventHandler(
                new InitialAltitudeEventHandler(
                    *persistence.sids,
                    *persistence.activeCallsigns,
                    *persistence.airfieldOwnership,
                    *persistence.login,
                    *persistence.deferredHandlers,
                    *persistence.plugin,
                    *persistence.flightplans
                )
            );

            persistence.initialAltitudeEvents = initialAltitudeEventHandler;
            persistence.userSettingHandlers->RegisterHandler(initialAltitudeEventHandler);
            persistence.flightplanHandler->RegisterHandler(initialAltitudeEventHandler);
            persistence.activeCallsigns->AddHandler(initialAltitudeEventHandler);


            TagFunction recycleFunction(
                recycleFunctionId,
                "Recycle Initial Altitude",
                std::bind(
                    &InitialAltitudeEventHandler::RecycleInitialAltitude,
                    *initialAltitudeEventHandler,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4
                )
            );
            persistence.pluginFunctionHandlers->RegisterFunctionCall(recycleFunction);
        }
    }  // namespace InitialAltitude
}  // namespace UKControllerPlugin
