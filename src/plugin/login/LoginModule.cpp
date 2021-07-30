#include "pch/pch.h"
#include "login/LoginModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "login/Login.h"
#include "timedevent/TimedEventCollection.h"
#include "plugin/UKPlugin.h"

using UKControllerPlugin::Controller::Login;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
namespace UKControllerPlugin {
    namespace Controller {

        /*
            Bootstrap the login module.
        */
        void LoginModule::BootstrapPlugin(UKControllerPlugin::Bootstrap::PersistenceContainer & persistence)
        {
            std::shared_ptr<Login> handler = std::make_shared<Login>(
                *persistence.plugin,
                *persistence.controllerHandler
            );

            persistence.login = handler;
            persistence.timedHandler->RegisterEvent(handler, 2);
            persistence.controllerHandler->RegisterHandler(handler);
        }
    }  // namespace Controller
}  // namespace UKControllerPlugin
