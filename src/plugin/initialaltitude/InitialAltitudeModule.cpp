#include "ClearInitialAltitude.h"
#include "InitialAltitudeModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "controller/ActiveCallsignCollection.h"
#include "departure/UserShouldClearDepartureDataEvent.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "eventhandler/EventBus.h"
#include "eventhandler/EventHandlerFlags.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "InitialAltitudeEventHandler.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/UKPlugin.h"
#include "tag/TagFunction.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::InitialAltitude::InitialAltitudeEventHandler;
using UKControllerPlugin::Tag::TagFunction;
using UKControllerPluginUtils::EventHandler::EventBus;

namespace UKControllerPlugin::InitialAltitude {

    const int timedHandlerFrequency = 10;

    /*
        Initialises the initial altitude module. Gets the altitudes from the dependency cache
        and registers the event handler to receive flightplan events.
    */
    void InitialAltitudeModule::BootstrapPlugin(PersistenceContainer& persistence)
    {
        std::shared_ptr<InitialAltitudeEventHandler> initialAltitudeEventHandler(new InitialAltitudeEventHandler(
            *persistence.sidMapper,
            *persistence.activeCallsigns,
            *persistence.airfieldOwnership,
            *persistence.login,
            *persistence.plugin));

        persistence.userSettingHandlers->RegisterHandler(initialAltitudeEventHandler);
        persistence.flightplanHandler->RegisterHandler(initialAltitudeEventHandler);
        persistence.activeCallsigns->AddHandler(initialAltitudeEventHandler);
        persistence.timedHandler->RegisterEvent(initialAltitudeEventHandler, timedHandlerFrequency);

        TagFunction recycleFunction(
            recycleFunctionId,
            "Recycle Initial Altitude",
            [initialAltitudeEventHandler](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                std::string context,
                const POINT& mousePos) {
                initialAltitudeEventHandler->RecycleInitialAltitude(fp, rt, std::move(context), mousePos);
            });
        persistence.pluginFunctionHandlers->RegisterFunctionCall(recycleFunction);

        // Register the clear initial altitude event handler
        EventBus::Bus().AddHandler<Departure::UserShouldClearDepartureDataEvent>(
            std::make_shared<ClearInitialAltitude>(*persistence.plugin, *persistence.sidMapper),
            UKControllerPluginUtils::EventHandler::EventHandlerFlags::Sync);
    }
} // namespace UKControllerPlugin::InitialAltitude
