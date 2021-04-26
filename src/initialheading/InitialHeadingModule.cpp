#include "pch/stdafx.h"
#include "initialheading/InitialHeadingModule.h"
#include "initialheading/InitialHeadingEventHandler.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "bootstrap/PersistenceContainer.h"
#include "tag/TagFunction.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::InitialHeading::InitialHeadingEventHandler;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Tag::TagFunction;
namespace UKControllerPlugin {
    namespace InitialHeading {

        // The function id for the recycle initial heading function
        const int recycleFunctionId = 9011;

        /*
            Initialises the initial altitude module. Gets the altitudes from the dependency cache
            and registers the event handler to receive flightplan events.
        */
        void BootstrapPlugin(
            PersistenceContainer & persistence
        ) {
            std::shared_ptr<InitialHeadingEventHandler> handler(
                new InitialHeadingEventHandler(
                    *persistence.sids,
                    *persistence.activeCallsigns,
                    *persistence.airfieldOwnership,
                    *persistence.login,
                    *persistence.deferredHandlers,
                    *persistence.plugin,
                    *persistence.flightplans
                )
            );
            persistence.userSettingHandlers->RegisterHandler(handler);
            persistence.flightplanHandler->RegisterHandler(handler);
            persistence.activeCallsigns->AddHandler(handler);


            TagFunction recycleFunction(
                recycleFunctionId,
                "Recycle Initial Heading",
                std::bind(
                    &InitialHeadingEventHandler::RecycleInitialHeading,
                    handler,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4
                )
            );
            persistence.pluginFunctionHandlers->RegisterFunctionCall(recycleFunction);
        }
    } // namespace InitialHeading
}  // namespace UKControllerPlugin
