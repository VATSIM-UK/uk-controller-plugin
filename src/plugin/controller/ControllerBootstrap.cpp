#include "pch/stdafx.h"
#include "controller/ControllerBootstrap.h"
#include "controller/ControllerPositionCollectionFactory.h"
#include "controller/ActiveCallsignMonitor.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;

namespace UKControllerPlugin {
    namespace Controller {

        void BootstrapPlugin(PersistenceContainer& container, DependencyLoaderInterface& dependency)
        {
            container.controllerPositions = std::move(
                ControllerPositionCollectionFactory::Create(dependency)
            );

            container.activeCallsigns.reset(new ActiveCallsignCollection);
            container.controllerHandler->RegisterHandler(
                std::make_shared<ActiveCallsignMonitor>(*container.controllerPositions, *container.activeCallsigns)
            );
        }
    }  // namespace Controller
}  // namespace UKControllerPlugin
