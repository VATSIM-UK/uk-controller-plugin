#include "SquawkAssignment.h"
#include "SquawkEventHandler.h"
#include "SquawkGenerator.h"
#include "SquawkModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/UKPlugin.h"
#include "squawk/ApiSquawkAllocationHandler.h"
#include "tag/TagFunction.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Squawk::ApiSquawkAllocationHandler;
using UKControllerPlugin::Squawk::SquawkEventHandler;
using UKControllerPlugin::Squawk::SquawkGenerator;
using UKControllerPlugin::Tag::TagFunction;

namespace UKControllerPlugin::Squawk {

    /*
        Bootstrap the squawk module when the plugin loads.
    */
    void SquawkModule::BootstrapPlugin(
        UKControllerPlugin::Bootstrap::PersistenceContainer& container, bool automaticAssignmentDisabled)
    {
        // API allocation handler
        std::shared_ptr<ApiSquawkAllocationHandler> allocations =
            std::make_shared<ApiSquawkAllocationHandler>(*container.plugin);
        container.timedHandler->RegisterEvent(allocations, SquawkModule::allocationCheckFrequency);

        // Assignment rules and generator
        container.squawkAssignmentRules = std::make_unique<SquawkAssignment>(
            *container.flightplans, *container.plugin, *container.airfieldOwnership, *container.activeCallsigns);

        container.squawkGenerator = std::make_unique<SquawkGenerator>(
            *container.api,
            container.taskRunner.get(),
            *container.squawkAssignmentRules,
            *container.activeCallsigns,
            *container.flightplans,
            allocations);

        // The event handler
        std::shared_ptr<SquawkEventHandler> eventHandler(new SquawkEventHandler(
            *container.squawkGenerator,
            *container.activeCallsigns,
            *container.flightplans,
            *container.plugin,
            *container.login,
            automaticAssignmentDisabled));

        container.squawkEvents = eventHandler;
        container.flightplanHandler->RegisterHandler(eventHandler);
        container.timedHandler->RegisterEvent(eventHandler, squawkAssignmentsCheckFrequency);
        container.userSettingHandlers->RegisterHandler(eventHandler);
        container.activeCallsigns->AddHandler(eventHandler);

        TagFunction forceSquawkCallbackGeneral(
            eventHandler->squawkForceCallbackIdGeneral,
            "Force Squawk Recycle (General)",
            [eventHandler](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                std::string context,
                const POINT& mousePos) { eventHandler->SquawkReycleGeneral(fp, rt, std::move(context), mousePos); });

        container.pluginFunctionHandlers->RegisterFunctionCall(forceSquawkCallbackGeneral);

        TagFunction forceSquawkCallbackLocal(
            eventHandler->squawkForceCallbackIdLocal,
            "Force Squawk Recycle (Local)",
            [eventHandler](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                std::string context,
                const POINT& mousePos) { eventHandler->SquawkRecycleLocal(fp, rt, std::move(context), mousePos); });
        
        container.pluginFunctionHandlers->RegisterFunctionCall(forceSquawkCallbackLocal);
    }
} // namespace UKControllerPlugin::Squawk
