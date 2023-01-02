#include "ActiveCallsignCollection.h"
#include "ActiveCallsignMonitor.h"
#include "ControllerBootstrap.h"
#include "ControllerPositionCollection.h"
#include "ControllerPositionCollectionFactory.h"
#include "ControllerPositionHierarchyFactory.h"
#include "ControllerStatusEventHandlerCollection.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyLoaderInterface.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;

namespace UKControllerPlugin::Controller {

    void BootstrapPlugin(PersistenceContainer& container, DependencyLoaderInterface& dependency)
    {
        container.controllerPositions = ControllerPositionCollectionFactory::Create(dependency);
        container.controllerHierarchyFactory =
            std::make_unique<ControllerPositionHierarchyFactory>(*container.controllerPositions);

        container.activeCallsigns = std::make_shared<ActiveCallsignCollection>();
        container.controllerHandler->RegisterHandler(
            std::make_shared<ActiveCallsignMonitor>(*container.controllerPositions, *container.activeCallsigns));
    }
} // namespace UKControllerPlugin::Controller
