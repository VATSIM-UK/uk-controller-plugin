#include "pch/stdafx.h"
#include "squawk/SquawkModule.h"
#include "squawk/SquawkEventHandler.h"
#include "squawk/SquawkGenerator.h"
#include "tag/TagFunction.h"
#include "bootstrap/PersistenceContainer.h"

using UKControllerPlugin::Squawk::SquawkEventHandler;
using UKControllerPlugin::Squawk::SquawkGenerator;
using UKControllerPlugin::Tag::TagFunction;

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
                container.plugin.get(),
                *container.squawkAssignmentRules,
                *container.activeCallsigns,
                *container.flightplans
            );

            std::shared_ptr<SquawkEventHandler> handler(
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
            container.squawkEvents = handler;
            container.flightplanHandler->RegisterHandler(handler);
            container.timedHandler->RegisterEvent(handler, SquawkModule::trackedAircraftCheckFrequency);
            container.userSettingHandlers->RegisterHandler(handler);

            TagFunction forceSquawkCallbackGeneral(
                handler->squawkForceCallbackIdGeneral,
                "Force Squawk Recycle (General)",
                std::bind(
                    &SquawkEventHandler::SquawkReycleGeneral,
                    *handler,
                    std::placeholders::_1,
                    std::placeholders::_2
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(forceSquawkCallbackGeneral);

            TagFunction forceSquawkCallbackLocal(
                handler->squawkForceCallbackIdLocal,
                "Force Squawk Recycle (Local)",
                std::bind(
                    &SquawkEventHandler::SquawkRecycleLocal,
                    *handler,
                    std::placeholders::_1,
                    std::placeholders::_2
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(forceSquawkCallbackLocal);
        }
    }  // namespace Squawk
}  // namespace UKControllerPlugin
