#include "ApiSquawkAllocationHandler.h"
#include "ResetSquawkOnFailedDelete.h"
#include "SquawkAssignment.h"
#include "SquawkAssignmentDeleteForConspicuityFailedEvent.h"
#include "SquawkAssignmentMenu.h"
#include "SquawkEventHandler.h"
#include "SquawkGenerator.h"
#include "SquawkModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "euroscope/CallbackFunction.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "eventhandler/EventBus.h"
#include "eventhandler/EventHandlerFlags.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/UKPlugin.h"
#include "tag/TagFunction.h"
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

        // Squawk assignment menu
        int squawkMenuCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        int manualSquawkEnteredCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        std::shared_ptr<SquawkAssignmentMenu> squawkMenu = std::make_shared<SquawkAssignmentMenu>(
            squawkMenuCallbackId,
            manualSquawkEnteredCallbackId,
            *container.squawkGenerator,
            *container.activeCallsigns,
            *container.plugin);

        // Register a callback function for when the option is selected
        Euroscope::CallbackFunction menuCallbackFunction(
            squawkMenuCallbackId,
            "Squawk Assignment Menu Callback",
            [squawkMenu](int, const std::string& context, const RECT& area) {
                squawkMenu->MenuOptionSelected(context, area);
            });
        container.pluginFunctionHandlers->RegisterFunctionCall(menuCallbackFunction);

        // Register a callback function for when a manual squawk is entered
        Euroscope::CallbackFunction manualSquawkEnteredCallbackFunction(
            manualSquawkEnteredCallbackId,
            "Manual Squawk Entered Callback",
            [squawkMenu](int, const std::string& context, const RECT& area) {
                squawkMenu->ManualSquawkEntered(context);
            });
        container.pluginFunctionHandlers->RegisterFunctionCall(manualSquawkEnteredCallbackFunction);

        // Register the tag function
        TagFunction squawkMenuTagFunction(
            9022,
            "Open Squawk Assignment Menu",
            [squawkMenu](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                const std::string& context,
                const POINT& mousePos) { squawkMenu->DisplaySquawkAssignmentMenu(fp, mousePos); });
        container.pluginFunctionHandlers->RegisterFunctionCall(squawkMenuTagFunction);

        // Handler to reset squawks if delete fails
        UKControllerPluginUtils::EventHandler::EventBus::Bus()
            .AddHandler<SquawkAssignmentDeleteForConspicuityFailedEvent>(
                std::make_shared<ResetSquawkOnFailedDelete>(*container.plugin, *container.flightplans),
                UKControllerPluginUtils::EventHandler::EventHandlerFlags::EuroscopeThread);
    }
} // namespace UKControllerPlugin::Squawk
