#include "pch/stdafx.h"
#include "handoff/HandoffModule.h"
#include "handoff/HandoffEventHandler.h"

using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin {
    namespace Handoff {

        const int handoffTagItem = 107;

        void BootstrapPlugin(
            PersistenceContainer& container,
            DependencyLoaderInterface& dependency
        ) {
            HandoffCollection handoffs;

            std::shared_ptr<HandoffEventHandler> handler = std::make_shared<HandoffEventHandler>(
                std::move(handoffs),
                *container.activeCallsigns
            );


            container.tagHandler->RegisterTagItem(107, handler);
            container.flightplanHandler->RegisterHandler(handler);
        }
    }  // namespace Handoff
}  // namespace UKControllerPlugin
