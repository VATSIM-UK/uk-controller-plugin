#include "pch/stdafx.h"
#include "squawk/SquawkModule.h"
#include "squawk/SquawkEventHandler.h"
#include "squawk/SquawkGenerator.h"
#include "tag/TagFunction.h"
#include "bootstrap/PersistenceContainer.h"
#include "squawk/ApiSquawkAllocationHandler.h"

using UKControllerPlugin::Squawk::SquawkEventHandler;
using UKControllerPlugin::Squawk::SquawkGenerator;
using UKControllerPlugin::Tag::TagFunction;
using UKControllerPlugin::Squawk::ApiSquawkAllocationHandler;

namespace UKControllerPlugin {
    namespace Squawk {

        /*
            Bootstrap the squawk module when the plugin loads.
        */
        void SquawkModule::BootstrapPlugin(
            UKControllerPlugin::Bootstrap::PersistenceContainer & container,
            bool disabled,
            bool automaticAssignmentDisabled
        ) {
            // API allocation handler
            std::shared_ptr<ApiSquawkAllocationHandler> allocations = std::make_shared<ApiSquawkAllocationHandler>(
                *container.plugin
            );
            container.timedHandler->RegisterEvent(allocations, SquawkModule::allocationCheckFrequency);

            // Assignment rules and generator
            container.squawkAssignmentRules.reset(
                new SquawkAssignment(
                *container.flightplans,
                *container.plugin,
                *container.airfieldOwnership,
                *container.activeCallsigns,
                disabled
            )
            );
            container.squawkGenerator = std::make_unique<SquawkGenerator>(
                *container.api,
                container.taskRunner.get(),
                *container.squawkAssignmentRules,
                *container.activeCallsigns,
                *container.flightplans,
                allocations
            );

            // The event handler
            std::shared_ptr<SquawkEventHandler> eventHandler(
                new SquawkEventHandler(
                    *container.squawkGenerator,
                    *container.activeCallsigns,
                    *container.flightplans,
                    *container.plugin,
                    *container.login,
                    *container.deferredHandlers,
                    automaticAssignmentDisabled
                )
            );

            container.squawkEvents = eventHandler;
            container.flightplanHandler->RegisterHandler(eventHandler);
            container.timedHandler->RegisterEvent(eventHandler, SquawkModule::trackedAircraftCheckFrequency);
            container.userSettingHandlers->RegisterHandler(eventHandler);

            TagFunction forceSquawkCallbackGeneral(
                eventHandler->squawkForceCallbackIdGeneral,
                "Force Squawk Recycle (General)",
                std::bind(
                    &SquawkEventHandler::SquawkReycleGeneral,
                    *eventHandler,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(forceSquawkCallbackGeneral);

            TagFunction forceSquawkCallbackLocal(
                eventHandler->squawkForceCallbackIdLocal,
                "Force Squawk Recycle (Local)",
                std::bind(
                    &SquawkEventHandler::SquawkRecycleLocal,
                    *eventHandler,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(forceSquawkCallbackLocal);
        }
    }  // namespace Squawk
}  // namespace UKControllerPlugin
