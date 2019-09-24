#include "pch/stdafx.h"
#include "bootstrap/EventHandlerCollectionBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "tag/TagItemCollection.h"
#include "euroscope/RadarTargetEventHandlerCollection.h"
#include "euroscope/RunwayDialogAwareCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "timedevent/TimedEventCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "metar/MetarEventHandlerCollection.h"
#include "timedevent/DeferredEventHandler.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "plugin/UKPlugin.h"
#include "command/CommandHandlerCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::Euroscope::RadarTargetEventHandlerCollection;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Metar::MetarEventHandlerCollection;
using UKControllerPlugin::TimedEvent::DeferredEventHandler;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::Euroscope::RunwayDialogAwareCollection;

namespace UKControllerPlugin {
    namespace Bootstrap {

        /*
            Set up all the event handler collections.
        */
        void EventHandlerCollectionBootstrap::BoostrapPlugin(PersistenceContainer & persistence)
        {
            persistence.tagHandler.reset(new TagItemCollection);
            persistence.radarTargetHandler.reset(new RadarTargetEventHandlerCollection);
            persistence.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
            persistence.controllerHandler.reset(new ControllerStatusEventHandlerCollection);
            persistence.timedHandler.reset(new TimedEventCollection);
            persistence.pluginFunctionHandlers.reset(new FunctionCallEventHandler);
            persistence.metarEventHandler.reset(new MetarEventHandlerCollection);
            persistence.deferredHandlers.reset(new DeferredEventHandler);
            persistence.userSettingHandlers.reset(new UserSettingAwareCollection);
            persistence.commandHandlers.reset(new CommandHandlerCollection);
            persistence.runwayDialogEventHandlers.reset(new RunwayDialogAwareCollection);

            persistence.timedHandler->RegisterEvent(persistence.deferredHandlers, 3);
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
