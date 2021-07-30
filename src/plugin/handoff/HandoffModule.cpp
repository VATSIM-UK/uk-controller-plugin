#include "pch/stdafx.h"
#include "handoff/HandoffModule.h"
#include "handoff/HandoffEventHandler.h"
#include "handoff/HandoffCollectionFactory.h"

using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin {
    namespace Handoff {

        const int handoffTagItem = 107;

        const std::string handoffOrdersDependencyKey = "DEPENDENCY_HANDOFF";
        const std::string handoffSidMappingsDependency = "DEPENDENCY_SID_HANDOFF";

        void BootstrapPlugin(
            PersistenceContainer& container,
            DependencyLoaderInterface& dependency
        ) {
            container.handoffs = Create(
                *container.controllerPositions,
                dependency.LoadDependency(handoffOrdersDependencyKey, "{}"_json),
                dependency.LoadDependency(handoffSidMappingsDependency, "{}"_json)
            );

            std::shared_ptr<HandoffEventHandler> handler = std::make_shared<HandoffEventHandler>(
                *container.handoffs,
                *container.activeCallsigns,
                *container.integrationModuleContainer->outboundMessageHandler
            );

            container.tagHandler->RegisterTagItem(107, handler);
            container.flightplanHandler->RegisterHandler(handler);
            container.activeCallsigns->AddHandler(handler);
        }
    }  // namespace Handoff
}  // namespace UKControllerPlugin
