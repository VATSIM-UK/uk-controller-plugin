#include "bootstrap/EventHandlerCollectionBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "command/CommandHandlerCollection.h"
#include "controller/HandoffEventHandlerCollection.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "euroscope/RadarTargetEventHandlerCollection.h"
#include "euroscope/RunwayDialogAwareCollection.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/UKPlugin.h"
#include "tag/TagItemCollection.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;
using UKControllerPlugin::Controller::HandoffEventHandlerCollection;
using UKControllerPlugin::Euroscope::RadarTargetEventHandlerCollection;
using UKControllerPlugin::Euroscope::RunwayDialogAwareCollection;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;

namespace UKControllerPlugin::Bootstrap {

    /*
        Set up all the event handler collections.
    */
    void EventHandlerCollectionBootstrap::BoostrapPlugin(PersistenceContainer& persistence)
    {
        persistence.tagHandler = std::make_unique<TagItemCollection>();
        persistence.radarTargetHandler = std::make_unique<RadarTargetEventHandlerCollection>();
        persistence.flightplanHandler = std::make_unique<FlightPlanEventHandlerCollection>();
        persistence.controllerHandler = std::make_unique<ControllerStatusEventHandlerCollection>();
        persistence.timedHandler = std::make_unique<TimedEventCollection>();
        persistence.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
        persistence.userSettingHandlers = std::make_unique<UserSettingAwareCollection>();
        persistence.commandHandlers = std::make_unique<CommandHandlerCollection>();
        persistence.runwayDialogEventHandlers = std::make_unique<RunwayDialogAwareCollection>();
        persistence.controllerHandoffHandlers = std::make_unique<HandoffEventHandlerCollection>();
    }
} // namespace UKControllerPlugin::Bootstrap
