#include "ApiSquawkAllocationHandler.h"
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
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Squawk::ApiSquawkAllocationHandler;
using UKControllerPlugin::Squawk::SquawkEventHandler;
using UKControllerPlugin::Squawk::SquawkGenerator;
using UKControllerPlugin::Tag::TagFunction;

namespace UKControllerPlugin::Squawk {

    // The callback function ID with euroscope for forcing squawk refresh (general squawk).
    const int squawkForceCallbackIdGeneral = 9000;

    // The callback function ID with euroscope for forcing squawk refresh (local squawk).
    const int squawkForceCallbackIdLocal = 9001;

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
            squawkForceCallbackIdGeneral,
            "Force Squawk Recycle (General)",
            [eventHandler](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                const std::string& context,
                const POINT& mousePos) { eventHandler->SquawkReycleGeneral(fp, rt, context, mousePos); });

        container.pluginFunctionHandlers->RegisterFunctionCall(forceSquawkCallbackGeneral);

        TagFunction forceSquawkCallbackLocal(
            squawkForceCallbackIdLocal,
            "Force Squawk Recycle (Local)",
            [eventHandler](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                const std::string& context,
                const POINT& mousePos) { eventHandler->SquawkRecycleLocal(fp, rt, context, mousePos); });

        container.pluginFunctionHandlers->RegisterFunctionCall(forceSquawkCallbackLocal);
    }
} // namespace UKControllerPlugin::Squawk
