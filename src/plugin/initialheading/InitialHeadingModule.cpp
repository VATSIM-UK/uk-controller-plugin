#include "bootstrap/PersistenceContainer.h"
#include "controller/ActiveCallsignCollection.h"
#include "departure/UserShouldClearDepartureDataEvent.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "eventhandler/EventBus.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "initialheading/ClearInitialHeading.h"
#include "initialheading/InitialHeadingEventHandler.h"
#include "initialheading/InitialHeadingModule.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/UKPlugin.h"
#include "tag/TagFunction.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::InitialHeading::InitialHeadingEventHandler;
using UKControllerPlugin::Tag::TagFunction;
using UKControllerPluginUtils::EventHandler::EventBus;
namespace UKControllerPlugin::InitialHeading {

    // The function id for the recycle initial heading function
    const int recycleFunctionId = 9011;

    const int timedHandlerFrequency = 10;

    /*
        Initialises the initial heading module. Gets the headings from the dependency cache
        and registers the event handler to receive flightplan events.
    */
    void BootstrapPlugin(PersistenceContainer& persistence)
    {
        std::shared_ptr<InitialHeadingEventHandler> handler(new InitialHeadingEventHandler(
            *persistence.sidMapper,
            *persistence.activeCallsigns,
            *persistence.airfieldOwnership,
            *persistence.login,
            *persistence.plugin));
        persistence.userSettingHandlers->RegisterHandler(handler);
        persistence.flightplanHandler->RegisterHandler(handler);
        persistence.activeCallsigns->AddHandler(handler);
        persistence.timedHandler->RegisterEvent(handler, timedHandlerFrequency);

        TagFunction recycleFunction(
            recycleFunctionId,
            "Recycle Initial Heading",
            [handler](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                std::string context,
                const POINT& mousePos) { handler->RecycleInitialHeading(fp, rt, std::move(context), mousePos); });
        persistence.pluginFunctionHandlers->RegisterFunctionCall(recycleFunction);

        // Register the clear initial heading event handler
        EventBus::Bus().AddHandler<Departure::UserShouldClearDepartureDataEvent>(
            std::make_shared<ClearInitialHeading>(*persistence.plugin, *persistence.sidMapper),
            UKControllerPluginUtils::EventHandler::EventHandlerFlags::Sync);
    }
} // namespace UKControllerPlugin::InitialHeading
